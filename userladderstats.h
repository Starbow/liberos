#ifndef USERLADDERSTATS_H
#define USERLADDERSTATS_H

#include <QObject>

class UserLadderStats : public QObject
{
	Q_OBJECT

public:
	UserLadderStats(QObject *parent, int wins, int losses, int forfeits, int walkovers, int points);
	~UserLadderStats();

	int wins() const;
	int losses() const;
	int forfeits() const;
	int walkovers() const;
	int points() const;

private:
	int wins_;
	int losses_;
	int forfeits_;
	int walkovers_;
	int points_;
};

#endif // USERLADDERSTATS_H
