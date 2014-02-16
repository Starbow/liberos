#ifndef LIBEROS_REQUESTS_CHATINDEXREQUEST_H
#define LIBEROS_REQUESTS_CHATINDEXREQUEST_H

#include "../request.h"
#include <QList>

class Eros;
class ChatRoom;

class ChatIndexRequest : public Request
{
	Q_OBJECT

public:
	enum RequestStatus
	{
		NotReady = -1,
		Success = 0,
		Fail = 1,
	};
	ChatIndexRequest(Eros *parent);
	~ChatIndexRequest();

	bool processData(const QString &command, const QByteArray &data);
	bool processError(int code, const QByteArray &data);

	const QList<ChatRoom *> &chatRooms() const;
	RequestStatus status() const;

private:
	

	QList<ChatRoom *> chatRooms_;
	RequestStatus status_;
};

#endif // LIBEROS_REQUESTS_CHATINDEXREQUEST_H
