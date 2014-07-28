#include "userladderstats.h"

UserLadderStats::UserLadderStats(QObject *parent, int wins, int losses, int forfeits, int walkovers, int points, int rating, int placements, Division *division, int division_rank)
	: QObject(parent)
{
	this->wins_ = wins;
	this->losses_ = losses;
	this->forfeits_ = forfeits;
	this->walkovers_ = walkovers;
	this->points_ = points;
	this->rating_ = rating;
	this->placements_ = placements;
	this->division_ = division;
    this->division_rank_ = division_rank;
}

UserLadderStats::~UserLadderStats()
{

}

int UserLadderStats::wins() const
{
	return this->wins_;
}
int UserLadderStats::losses() const
{
	return this->losses_;
}
int UserLadderStats::forfeits() const
{
	return this->forfeits_;
}
int UserLadderStats::walkovers() const
{
	return this->walkovers_;
}
int UserLadderStats::points() const
{
	return this->points_;
}
int UserLadderStats::rating() const
{
	return this->rating_;
}
int UserLadderStats::placements() const
{
	return this->placements_;
}
Division * UserLadderStats::division() const
{
	return this->division_;
}
int UserLadderStats::divisionRank() const
{
    return this->division_rank_;
}
