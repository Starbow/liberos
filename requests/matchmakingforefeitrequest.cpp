#include "matchmakingforefeitrequest.h"

MatchmakingForefeitRequest::MatchmakingForefeitRequest(Eros *parent)
	: Request(parent, "MMF")
{

}

MatchmakingForefeitRequest::~MatchmakingForefeitRequest()
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