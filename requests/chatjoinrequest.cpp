#include "chatjoinrequest.h"
#include "../eros.pb.h"

ChatJoinRequest::ChatJoinRequest(Eros *parent, ChatRoom *room, const QString &password)
	: Request(parent, "UCJ")
{
	protobufs::ChatRoomRequest message;
	message.set_room(room->name().toStdString());
	message.set_password(password.toStdString());

	this->data_ = serializeMessageToBuffer(&message);
	this->data_->open(QIODevice::ReadOnly);

	this->status_ = RequestStatus::NotReady;
	this->room_ = room;
}

ChatJoinRequest::~ChatJoinRequest()
{
	this->data_->close();
	delete this->data_;
	this->data_ = nullptr;
}

bool ChatJoinRequest::processData(const QString &command, const QByteArray &data)
{
	if (command == "UCJ")
	{
		protobufs::ChatRoomInfo info;
		info.ParseFromArray(data.data(), data.size());

		this->room_->update(info);
		this->status_ = RequestStatus::Success;

		emit complete(this);
		return true;
	}
	return false;
}

bool ChatJoinRequest::processError(int code, const QByteArray &data)
{
	if (this->status_ == RequestStatus::NotReady)
	{
		
		this->status_ = (RequestStatus)code;
		emit complete(this);
		return true;
	}
	return false;

}

ChatJoinRequest::RequestStatus ChatJoinRequest::status() const
{
	return this->status_;
}

ChatRoom *ChatJoinRequest::room() const
{
	return this->room_;
}