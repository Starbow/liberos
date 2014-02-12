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
