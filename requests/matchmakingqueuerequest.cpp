#include "matchmakingqueuerequest.h"
#include "../eros.pb.h"
#include "../matchmakingmatch.h"


MatchmakingQueueRequest::MatchmakingQueueRequest(Eros *parent, ErosRegion region, int search_radius)
	: Request(parent, "MMQ")
{
	protobufs::MatchmakingQueue queue;
	queue.set_region((protobufs::Region)region);
	queue.set_radius(search_radius);
	
	this->data_ = serializeMessageToBuffer(&queue);
	this->data_->open(QIODevice::ReadOnly);

	this->status_ = ErosMatchmakingState::Idle; 
	this->match_ = nullptr;
	this->matchmaking_timer_ = new QTime();
}

MatchmakingQueueRequest::~MatchmakingQueueRequest()
{
	this->data_->close();
	delete this->data_;
	delete this->matchmaking_timer_;
	this->data_ = nullptr;
	this->match_ = nullptr;
}

void MatchmakingQueueRequest::requestSent()
{
	matchmaking_timer_->restart();
}

bool MatchmakingQueueRequest::processData(const QString &command, const QByteArray &data)
{
	if (command == "MMQ")
	{
		this->status_ = ErosMatchmakingState::Queued;
		emit queued(this);
		return true;
	}
	else if (command == "MMR")
	{
		int time = matchmaking_timer_->elapsed();
		protobufs::MatchmakingResult result;
		result.ParseFromArray(data.data(), data.size());

		this->match_ = new MatchmakingMatch(this->eros_, result);
		this->status_ = ErosMatchmakingState::Matched;

		emit complete(this);
		return true;
	}

	emit unhandled(this, command);
	return false;
}

bool MatchmakingQueueRequest::processError(int code, const QByteArray &data)
{
	if (code == 401 || code == 402)
	{
		this->status_ = (ErosMatchmakingState)code;
		emit complete(this);
		return true;
	}

	return false;
}

ErosMatchmakingState MatchmakingQueueRequest::status() const
{
	return this->status_;
}


MatchmakingMatch *MatchmakingQueueRequest::match() const
{
	return this->match_;
}