#include "matchmakingforefeitrequest.h"

MatchmakingForefeitRequest::MatchmakingDequeueRequest(Eros *parent)
	: Request(parent, "MMF")
{

}

MatchmakingForefeitRequest::~MatchmakingDequeueRequest()
{

}

bool MatchmakingForefeitRequest::processData(const QString &command, const QByteArray &data)
{
	if (command == "MMF")
	{
		emit complete(this);
		return true;
	}

	return false;
}