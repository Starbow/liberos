#ifndef MATCHMAKINGMATCH_H
#define MATCHMAKINGMATCH_H

#include <QObject>

class User;
class Eros;
class ChatRoom;
//class Map;

namespace protobufs
{
	class MatchmakingResult;
}

class MatchmakingMatch : public QObject
{
	Q_OBJECT

public:
	MatchmakingMatch(Eros *parent, const protobufs::MatchmakingResult &result);
	~MatchmakingMatch();

private:
	User *opponent_;
	int match_time_;
	ChatRoom *chat_room_;
	QString battle_net_channel_;
	int opponent_latency_;
	QString map_;

};

#endif // MATCHMAKINGMATCH_H
