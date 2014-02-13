#include "updatecharacterrequest.h"
#include "../eros.pb.h"

UpdateCharacterRequest::UpdateCharacterRequest(Eros *parent, Character *character, int new_character_code, const QString new_game_profile_link)
	: Request(parent, "BNU")
{
	protobufs::Character character_message;
	character_message.set_character_name(character->displayName().toStdString());
	character_message.set_region((protobufs::Region)character->region());
	character_message.set_subregion(character->realm());
	character_message.set_profile_id(character->profileId());

	if (new_character_code > 0)
	{
		character_message.set_character_code(new_character_code);
	}

	if (!new_game_profile_link.isNull())
	{
		character_message.set_ingame_profile_link(new_game_profile_link.toStdString());
	}

	this->data_ = serializeMessageToBuffer(&character_message);
	this->data_->open(QIODevice::ReadOnly);

	this->status_ = RequestStatus::NotReady; 
	this->character_ = character;
}

UpdateCharacterRequest::~UpdateCharacterRequest()
{
	this->data_->close();
	delete this->data_;
	this->data_ = nullptr;
}

UpdateCharacterRequest::RequestStatus UpdateCharacterRequest::status() const
{
	return this->status_;
}
Character *UpdateCharacterRequest::character() const
{
	return this->character_;
}

bool UpdateCharacterRequest::processData(const QString &command, const QByteArray &data)
{
	if (command == "BNU")
	{
		protobufs::Character character_message;
		character_message.ParseFromArray(data.data(), data.size());

		this->status_ = RequestStatus::Success;
		this->character_->update(character_message);
		emit complete(this);
		return true;
	}

	return false;
}

bool UpdateCharacterRequest::processError(int code, const QByteArray &data)
{
	if (this->status_ == RequestStatus::NotReady)
	{
		this->status_ = (RequestStatus)code;
		emit complete(this);
		return true;
	}

	return false;
}
