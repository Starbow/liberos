#include "user.h"
#include "eros.h"
#include "eros.pb.h"

User::User(Eros *parent, const QString &username)
	: QObject(0)
{
	this->eros_ = parent;
	this->username_ = username;
	this->first_update_ = true;
	this->state_ = ErosUserState::Unknown;
}

User::User(Eros *parent)
	:QObject(parent)
{
	this->state_ = ErosUserState::Unknown;
	this->first_update_ = true;
	this->eros_ = parent;
}
User::~User()
{

}
ErosUserState User::state() const
{
	return this->state_;
}

const QString &User::username() const
{
	return this->username_;
}
int User::searchRadius() const
{
	return this->search_radius_;
}


const QMap<ErosRegion, UserLadderStats*> &User::ladderStats() const
{
	return this->ladder_stats_;
}
const UserLadderStats *User::ladderStatsGlobal() const
{
	return this->ladder_stats_global_;
}

void User::update(const protobufs::UserStats &stats)
{
	this->state_ = ErosUserState::Known;
	this->username_ = QString::fromStdString(stats.username());
	this->ladder_stats_global_ = new UserLadderStats(this, stats.wins(), stats.losses(), stats.forefeits(), stats.walkovers(), stats.points());
	this->search_radius_ = stats.search_radius();
	
	for (int i = 0; i < stats.region_size(); i++)
	{
		const protobufs::UserRegionStats &region = stats.region(i);
		this->ladder_stats_[(ErosRegion)region.region()] = new UserLadderStats(this, region.wins(), region.losses(), region.forefeits(), region.walkovers(), region.points());
	}

	if (this->first_update_)
	{
		this->first_update_ = false;
	} 
	else
	{
		emit updated(this);
	}	
}