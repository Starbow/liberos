#include "privatemessagerequest.h"
#include "../eros.pb.h"

PrivateMessageRequest::PrivateMessageRequest(Eros *parent, User *user, const QString &message)
	: Request(parent, "UPM")
{
	protobufs::ChatMessage chat_message;
	chat_message.set_sender(parent->localUser()->username().toStdString());
	chat_message.set_target(user->username().toStdString());
	chat_message.set_message(message.toStdString());

	this->data_ = serializeMessageToBuffer(&chat_message);
	this->data_->open(QIODevice::ReadOnly);
	this->status_ = RequestStatus::NotReady;

	this->user_ = user;
	this->message_ = message;
}



PrivateMessageRequest::~PrivateMessageRequest()
{
	this->data_->close();
	delete this->data_;
	this->data_ = nullptr;
}

PrivateMessageRequest::RequestStatus PrivateMessageRequest::status() const
{
	return this->status_;
}

bool PrivateMessageRequest::processData(const QString &command, const QByteArray &data)
{
	if (command == "UPM")
	{
		this->status_ = RequestStatus::Success;
		emit complete(this);
		return true;
	}

	return false;
}

bool PrivateMessageRequest::processError(int code, const QByteArray &data)
{
	this->status_ = (RequestStatus)code;
	emit complete(this);
	return true;
}

User *PrivateMessageRequest::user() const
{
	return this->user_;
}

const QString &PrivateMessageRequest::message() const
{
	return this->message_;
}