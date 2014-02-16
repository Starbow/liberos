#ifndef LIBEROS_REQUESTS_MATCHMAKINGDEQUEUEREQUEST_H
#define LIBEROS_REQUESTS_MATCHMAKINGDEQUEUEREQUEST_H

#include "../request.h"

class MatchmakingDequeueRequest : public Request
{
	Q_OBJECT

public:
	MatchmakingDequeueRequest(Eros *parent);
	~MatchmakingDequeueRequest();

	bool processData(const QString &command, const QByteArray &data);
private:
	
};

#endif // LIBEROS_REQUESTS_MATCHMAKINGDEQUEUEREQUEST_H
