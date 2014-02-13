#ifndef MATCHMAKINGDEQUEUEREQUEST_H
#define MATCHMAKINGDEQUEUEREQUEST_H

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

#endif // MATCHMAKINGDEQUEUEREQUEST_H
