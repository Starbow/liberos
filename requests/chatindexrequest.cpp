#include "chatindexrequest.h"
#include "../eros.pb.h"

ChatIndexRequest::ChatIndexRequest(Eros *parent)
	: Request(parent, "UCI")
{
	this->status_ = RequestStatus::NotReady;
	this->chatRooms_ = QList<ChatRoom *>();
}



ChatIndexRequest::~ChatIndexRequest()
{

}

bool ChatIndexRequest::processData(const QString &command, const QByteArray &data)
{
	if (command == "UCI")
	{
		protobufs::ChatRoomIndex index;
		index.ParseFromArray(data.data(), data.size());

		for (int i = 0; i < index.room_size(); i++)
		{
			const protobufs::ChatRoomInfo &info = index.room(i);

			ChatRoom *room = this->eros_->getChatRoom(QString::fromStdString(info.name()));
			room->update(info);

			this->chatRooms_ << room;
		}

		this->status_ = RequestStatus::Success;
		emit complete(this);
		return true;
	}

	return false;
}

bool ChatIndexRequest::processError(int code, const QByteArray &data)
{
	if (this->status_ == RequestStatus::NotReady)
	{
		this->status_ = RequestStatus::Fail;
		emit complete(this);
		return true;
	}
	return false;
}

const QList<ChatRoom *> &ChatIndexRequest::chatRooms() const
{
	return this->chatRooms_;
}

ChatIndexRequest::RequestStatus ChatIndexRequest::status() const
{
	return this->status_;
}