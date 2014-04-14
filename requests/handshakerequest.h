#ifndef LIBEROS_REQUESTS_HANDSHAKEREQUEST_H
#define LIBEROS_REQUESTS_HANDSHAKEREQUEST_H

#include "../request.h"
#include <QBuffer>

class Map;
class HandshakeRequest : public Request
{
	Q_OBJECT


public:

	enum ResponseStatus
	{
		NotReady = -1,
		Fail = 0,
		Success = 1,
		AlreadyLoggedIn = 2,
	};

	HandshakeRequest(Eros *parent, const QString &username, const QString &password);
	~HandshakeRequest();

	bool processData(const QString &command, const QByteArray &data);
	

	LocalUser *user() const;
	const QMap<int, Division*> &divisions() const;
	ResponseStatus status() const;
	const QList<ErosRegion> &activeRegions() const;
	const QList<Map*> &mapPool() const;
	int maxVetoes() const;

private:
	
	ResponseStatus status_;
	LocalUser *user_;
	QMap<int, Division*> divisions_;
	QList<ErosRegion> active_regions_;
	QList<Map*> map_pool_;
	int max_vetoes_;

	bool processHandshakeResponse(const QString &command, const QByteArray &data);
};

#endif // LIBEROS_REQUESTS_HANDSHAKEREQUEST_H

