#include "chatmessagerequest.h"
#include "../eros.pb.h"

ChatMessageRequest::ChatMessageRequest(Eros *parent, ChatRoom *room, const QString &message)
	: Request(parent, "UCM")
{
	protobufs::ChatMessage chat_message;
	chat_message.set_sender(parent->localUser()->username().toStdString());
	chat_message.set_target(room->name().toStdString());
	chat_message.set_message(message.toStdString());

	this->data_ = serializeMessageToBuffer(&chat_message);
	this->data_->open(QIODevice::ReadOnly);
	this->status_ = RequestStatus::NotReady;

	this->room_ = room;
	this->message_ = message;
}



ChatMessageRequest::~ChatMessageRequest()
{
	this->data_->close();
	delete this->data_;
	this->data_ = nullptr;
}

ChatMessageRequest::RequestStatus ChatMessageRequest::status() const
{
	return this->status_;
}

bool ChatMessageRequest::processData(const QString &command, const QByteArray &data)
{
	if (command == "UCM")
	{
		this->status_ = RequestStatus::Success;
		emit complete(this);
		return true;
	}

	return false;
}

bool ChatMessageRequest::processError(int code, const QByteArray &data)
{
	this->status_ = (RequestStatus)code;
	emit complete(this);
	return true;
}

ChatRoom *ChatMessageRequest::room() const
{
	return this->room_;
}

const QString &ChatMessageRequest::message() const
{
	return this->message_;
}