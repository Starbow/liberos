#ifndef LIBEROS_MATCHMAKINGMATCH_H
#define LIBEROS_MATCHMAKINGMATCH_H

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

	User *opponent() const;
	int matchTime() const;
	ChatRoom *chatRoom() const;
	const QString &battleNetChannel() const;
	int opponentLatency() const;
	const QString &map() const;

private:
	User *opponent_;
	int match_time_;
	ChatRoom *chat_room_;
	QString battle_net_channel_;
	int opponent_latency_;
	QString map_;

};

#endif // LIBEROS_MATCHMAKINGMATCH_H
