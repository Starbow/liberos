#include "userladderstats.h"

UserLadderStats::UserLadderStats(QObject *parent, int wins, int losses, int forfeits, int walkovers, int points)
	: QObject(parent)
{
	this->wins_ = wins;
	this->losses_ = losses;
	this->forfeits_ = forfeits;
	this->walkovers_ = walkovers;
	this->points_ = points;
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
