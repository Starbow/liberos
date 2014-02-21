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
	const protobufs::Map &map = result.map();
	this->map_name_ = QString::fromStdString(map.battle_net_name());
	this->map_id_ = map.battle_net_id();
	this->long_process_unlock_time_ = result.long_unlock_time();
	this->long_process_response_time_ = result.long_response_time();
}

MatchmakingMatch::~MatchmakingMatch()
{

}

int MatchmakingMatch::longProcessUnlockTime() const
{
	return this->long_process_unlock_time_;
}

int MatchmakingMatch::longProcessResponseTime() const
{
	return this->long_process_response_time_;
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
const QString &MatchmakingMatch::mapName() const
{
	return this->map_name_;
}

int MatchmakingMatch::mapId() const
{
	return this->map_id_;
}