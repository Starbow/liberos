#ifndef PRIVATEMESSAGEREQUEST_H
#define PRIVATEMESSAGEREQUEST_H

#include "../request.h"
#include <QObject>


class User;

class PrivateMessageRequest : public Request
{
	Q_OBJECT

public:

	enum RequestStatus
	{
		NotReady = -1,
		Success = 0,
		BadMessage = 508,
		UserOffline = 507,
	};
	PrivateMessageRequest(Eros *parent, User *user, const QString &message);
	~PrivateMessageRequest();

	bool processData(const QString &command, const QByteArray &data);
	bool processError(int code, const QByteArray &data);

	RequestStatus status() const;

	User *user() const;
	const QString &message() const;
private:
	
	RequestStatus status_;
	User *user_;
	QString message_;
};

#endif // PRIVATEMESSAGEREQUEST_H
