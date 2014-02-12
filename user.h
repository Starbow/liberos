#ifndef USER_H
#define USER_H

#include <QObject>
#include <QString>
#include <QVector>

#include "enums.h"

class Eros;
namespace protobufs
{
	class HandshakeResponse;
	class UserStats;
};

class User : public QObject
{
	Q_OBJECT

public:
	User(const Eros *parent, const QString &username);
	~User();

	const QString &username() const;
	int wins() const;
	int losses() const;
	int forefeits() const;
	int walkovers() const;
	int searchRadius() const;
	int points() const;
	void update(const protobufs::UserStats &stats);
	ErosUserState state() const;

signals:
	void updated(User* user);

protected:
	User(Eros *parent);

	QString username_;
	int wins_;
	int losses_;
	int forefeits_;
	int walkovers_;
	int search_radius_;
	int points_;

	bool first_update_;

	ErosUserState state_;
};

#endif // USER_H
