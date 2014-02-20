#ifndef LIBEROS_REQUESTS_MATCHMAKINGFORFEITREQUEST_H
#define LIBEROS_REQUESTS_MATCHMAKINGFORFEITREQUEST_H

#include "../request.h"

class MatchmakingForefeitRequest : public Request
{
	Q_OBJECT

public:
	MatchmakingForefeitRequest(Eros *parent);
	~MatchmakingForefeitRequest();

	bool processData(const QString &command, const QByteArray &data);
private:
	
};

#endif // LIBEROS_REQUESTS_MATCHMAKINGFORFEITREQUEST_H
