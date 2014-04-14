#ifndef USERLADDERSTATS_H
#define USERLADDERSTATS_H

#include <QObject>
class Division;

class UserLadderStats : public QObject
{
	Q_OBJECT

public:
	UserLadderStats(QObject *parent, int wins, int losses, int forfeits, int walkovers, int points, int rating, int placements, Division *division);
	~UserLadderStats();

	int wins() const;
	int losses() const;
	int forfeits() const;
	int walkovers() const;
	int points() const;
	int rating() const;
	Division *division() const;
	int placements() const;

private:
	int wins_;
	int losses_;
	int forfeits_;
	int walkovers_;
	int points_;
	int rating_;
	int placements_;
	Division *division_;
};

#endif // USERLADDERSTATS_H
