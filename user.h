#ifndef LIBEROS_USER_H
#define LIBEROS_USER_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QList>

#include "userladderstats.h"

#include "enums.h"

class Eros;
class Map;
namespace protobufs
{
	class HandshakeResponse;
	class UserStats;
	class MapPool;
};

class User : public QObject
{
	Q_OBJECT

public:
	User(Eros *parent, const QString &username);
	~User();

	const QString &username() const;
	int searchRadius() const;
	void update(const protobufs::UserStats &stats);
	void update(const protobufs::UserStats &stats, const QList<Map*> &map_pool);
	void update(const protobufs::MapPool &vetoes);
	void update(const protobufs::MapPool &vetoes, const QList<Map*> &map_pool);
	ErosUserState state() const;
	const QMap<ErosRegion, UserLadderStats*> &ladderStats() const;
	const UserLadderStats *ladderStatsGlobal() const;
	const QList<Map*> &vetoes() const;

signals:
	void updated(User* user);

protected:
	User(Eros *parent);

	QString username_;
	
	int search_radius_;
	Eros *eros_;
	bool first_update_;
	UserLadderStats *ladder_stats_global_;
	QMap<ErosRegion, UserLadderStats*> ladder_stats_;
	QList<Map*> vetoes_;

	ErosUserState state_;
};

#endif // LIBEROS_USER_H
