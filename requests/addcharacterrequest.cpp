#include "addcharacterrequest.h"
#include "../eros.pb.h"

AddCharacterRequest::AddCharacterRequest(Eros *parent, const QString &battle_net_url)
	: Request(parent, "BNA")
{
	data_ = new QBuffer();
	data_->open(QIODevice::ReadWrite);
	data_->write(battle_net_url.toUtf8());
	data_->close();
	data_->open(QIODevice::ReadOnly);

	this->status_ = RequestStatus::NotReady;
	this->battle_net_url_ = battle_net_url;
	this->character_ = nullptr;
}

AddCharacterRequest::~AddCharacterRequest()
{
	this->data_->close();
	delete this->data_;
	this->data_ = nullptr;
}

AddCharacterRequest::RequestStatus AddCharacterRequest::status() const
{
	return this->status_;
}
Character *AddCharacterRequest::character() const
{
	return this->character_;
}
const QString &AddCharacterRequest::battle_net_url() const
{
	return this->battle_net_url_;
}

bool AddCharacterRequest::processData(const QString &command, const QByteArray &data)
{
	if (command == "BNA")
	{
		protobufs::Character character_message;
		character_message.ParseFromArray(data.data(), data.size());

		this->status_ = RequestStatus::Success;
		this->character_ = new Character(this->eros_, character_message);
		emit complete(this);
		return true;
	}

	return false;
}

bool AddCharacterRequest::processError(int code, const QByteArray &data)
{
	if (this->status_ == RequestStatus::NotReady)
	{
		this->status_ = (RequestStatus)code;
		emit complete(this);
		return true;
	}

	return false;
}