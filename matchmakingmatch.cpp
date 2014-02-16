#include "matchmakingmatch.h"
#include "eros.h"
#include "eros.pb.h"

MatchmakingMatch::MatchmakingMatch(Eros *parent, const protobufs::MatchmakingResult &result)
	: QObject(parent)
{
	const protobufs::UserStats &opponent_stats = result.opponent();
	User *user = parent->getUser(QString::fromStdString(opponent_stats.username()));
	user->update(opponent_stats);

	this->opponent_ = user;
	this->match_time_ = result.timespan();
	this->battle_net_channel_ = QString::fromStdString(result.channel());
	this->chat_room_ = parent->getChatRoom(QString::fromStdString(result.chat_room()));
	this->opponent_latency_ = result.opponent_latency();
}

MatchmakingMatch::~MatchmakingMatch()
{

}

User *MatchmakingMatch::opponent() const
{
	return this->opponent_;
}
int MatchmakingMatch::matchTime() const
{
	return this->match_time_;
}
ChatRoom *MatchmakingMatch::chatRoom() const
{
	return this->chat_room_;
}
const QString &MatchmakingMatch::battleNetChannel() const
{
	return this->battle_net_channel_;
}
int MatchmakingMatch::opponentLatency() const
{
	return this->opponent_latency_;
}
const QString &MatchmakingMatch::map() const
{
	return this->map_;
}