#include "chatleaverequest.h"
#include "../eros.pb.h"

ChatLeaveRequest::ChatLeaveRequest(Eros *parent, ChatRoom *room)
	: Request(parent, "UCL")
{
	protobufs::ChatRoomRequest message;
	message.set_room(room->name().toStdString());
	message.set_password("");

	this->data_ = serializeMessageToBuffer(&message);
	this->data_->open(QIODevice::ReadOnly);

	this->room_ = room;
}

ChatLeaveRequest::~ChatLeaveRequest()
{
	this->data_->close();
	delete this->data_;
	this->data_ = nullptr;
}

bool ChatLeaveRequest::processData(const QString &command, const QByteArray &data)
{
	if (command == "UCL")
	{
		emit complete(this);
		return true;
	}

	return false;
}

ChatRoom *ChatLeaveRequest::room() const
{
	return this->room_;
}