#include "matchmakingdequeuerequest.h"

MatchmakingDequeueRequest::MatchmakingDequeueRequest(Eros *parent)
	: Request(parent, "MMD")
{

}

MatchmakingDequeueRequest::~MatchmakingDequeueRequest()
{

}

bool MatchmakingDequeueRequest::processData(const QString &command, const QByteArray &data)
{
	if (command == "MMD")
	{
		emit complete(this);
		return true;
	}

	return false;
}