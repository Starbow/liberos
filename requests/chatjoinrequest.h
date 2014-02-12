#ifndef CHATJOINREQUEST_H
#define CHATJOINREQUEST_H

#include "../request.h"

class ChatJoinRequest : public Request
{
	Q_OBJECT

public:
	enum RequestStatus
	{
		NotReady = -1,
		Success = 0,
		TooManyRooms = 505,
		RoomNotJoinable = 501,
		BadPassword = 502,
		RoomAlreadyExists = 503,
		RoomReserved = 504,
		RoomNameTooShort = 509,
	};

	ChatJoinRequest(Eros *parent, ChatRoom *room, const QString &password);
	~ChatJoinRequest();

	bool processData(const QString &command, const QByteArray &data);
	bool processError(int code, const QByteArray &data);

	RequestStatus status() const;
	ChatRoom *room() const;
private:
	ChatRoom *room_;
	RequestStatus status_;
};

#endif // CHATJOINREQUEST_H
