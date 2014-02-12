#ifndef HANDSHAKEREQUEST_H
#define HANDSHAKEREQUEST_H

#include "../request.h"
#include <QBuffer>

class HandshakeRequest : public Request
{
	Q_OBJECT


public:

	enum ResponseStatus
	{
		NotReady = -1,
		Fail = 0,
		Success = 1,
	};

	HandshakeRequest(Eros *parent, const QString &username, const QString &password);
	~HandshakeRequest();

	bool processData(const QString &command, const QByteArray &data);
	

	LocalUser *user() const;
	ResponseStatus status() const;

	


private:
	
	ResponseStatus status_;
	LocalUser *user_;
	
	bool processHandshakeResponse(const QString &command, const QByteArray &data);
};

#endif // HANDSHAKEREQUEST_H

