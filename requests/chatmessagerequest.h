#ifndef LIBEROS_REQUESTS_CHATMESSAGEREQUEST_H
#define LIBEROS_REQUESTS_CHATMESSAGEREQUEST_H

#include "../request.h"
#include <QObject>


class User;

class ChatMessageRequest : public Request
{
	Q_OBJECT

public:

	enum RequestStatus
	{
		NotReady = -1,
		Success = 0,
		BadMessage = 508,
		NotOnChannel = 506,
	};
	ChatMessageRequest(Eros *parent, ChatRoom *room, const QString &message);
	~ChatMessageRequest();

	bool processData(const QString &command, const QByteArray &data);
	bool processError(int code, const QByteArray &data);

	RequestStatus status() const;

	
	ChatRoom *room() const;
	const QString &message() const;

private:
	
	RequestStatus status_;
	ChatRoom *room_;
	QString message_;
};

#endif // LIBEROS_REQUESTS_CHATMESSAGEREQUEST_H
