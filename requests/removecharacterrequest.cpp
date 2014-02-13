#include "removecharacterrequest.h"
#include "../eros.pb.h"

RemoveCharacterRequest::RemoveCharacterRequest(Eros *parent, Character *character)
	: Request(parent, "BNR")
{
	protobufs::Character character_message;
	character_message.set_character_name(character->displayName().toStdString());
	character_message.set_region((protobufs::Region)character->region());
	character_message.set_subregion(character->realm());
	character_message.set_profile_id(character->profileId());


	this->data_ = serializeMessageToBuffer(&character_message);
	this->data_->open(QIODevice::ReadOnly);

	this->status_ = RequestStatus::NotReady; 
	this->character_ = character;
}

RemoveCharacterRequest::~RemoveCharacterRequest()
{
	this->data_->close();
	delete this->data_;
	this->data_ = nullptr;
}


RemoveCharacterRequest::RequestStatus RemoveCharacterRequest::status() const
{
	return this->status_;
}
Character *RemoveCharacterRequest::character() const
{
	return this->character_;
}

bool RemoveCharacterRequest::processData(const QString &command, const QByteArray &data)
{
	if (command == "BNR")
	{
		this->status_ = RequestStatus::Success;
		emit complete(this);
		return true;
	}

	return false;
}

bool RemoveCharacterRequest::processError(int code, const QByteArray &data)
{
	if (this->status_ == RequestStatus::NotReady)
	{
		this->status_ = (RequestStatus)code;
		emit complete(this);
		return true;
	}

	return false;
}
