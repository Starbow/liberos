#ifndef MATCHMAKINGDEQUEUEREQUEST_H
#define MATCHMAKINGDEQUEUEREQUEST_H

#include "../request.h"

class MatchmakingDequeueRequest : public Request
{
	Q_OBJECT

public:
	MatchmakingDequeueRequest(Eros *parent);
	~MatchmakingDequeueRequest();

private:
	
};

#endif // MATCHMAKINGDEQUEUEREQUEST_H
