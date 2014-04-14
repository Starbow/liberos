#include "user.h"
#include "eros.h"
#include "eros.pb.h"

User::User(Eros *parent, const QString &username)
	: QObject(0)
{
	this->eros_ = parent;
	this->username_ = username;
	this->id_ = 0;
	this->first_update_ = true;
	this->ladder_stats_global_ = nullptr;
	this->division_ = nullptr;

	this->state_ = ErosUserState::Unknown;
}

User::User(Eros *parent)
	:QObject(parent)
{
	this->state_ = ErosUserState::Unknown;
	this->first_update_ = true;
	this->ladder_stats_global_ = nullptr;
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

const QList<Map*> &User::vetoes() const
{
	return this->vetoes_;
}

Division *User::division() const
{
	return this->division_;
}

qint64 User::id() const
{
	return this->id_;
}

void User::update(const protobufs::UserStats &stats, const QList<Map*> &map_pool, const QMap<int, Division*> &divisions)
{
	this->state_ = ErosUserState::Known;
	this->username_ = QString::fromStdString(stats.username());
	this->id_ = stats.id();
	this->ladder_stats_global_ = new UserLadderStats(this, stats.wins(), stats.losses(), stats.forfeits(), stats.walkovers(), stats.points(), stats.mmr() * 100, stats.placements_remaining(), divisions[stats.division()]);
	this->division_ = this->ladder_stats_global_->division();
	this->search_radius_ = stats.search_radius();
	for (int i = 0; i < stats.region_size(); i++)
	{
		const protobufs::UserRegionStats &region = stats.region(i);
		this->ladder_stats_[(ErosRegion)region.region()] = new UserLadderStats(this, region.wins(), region.losses(), region.forfeits(), region.walkovers(), region.points(), region.mmr() * 100, region.placements_remaining(), divisions[region.division()]);
	}

	this->vetoes_.clear();

	for (int i = 0; i < stats.vetoes_size(); i++)
	{
		const protobufs::Map &map_buffer = stats.vetoes(i);
		for (int j = 0; j < map_pool.size(); j++)
		{
			if (map_pool[j]->battleNetId() == map_buffer.battle_net_id() && map_pool[j]->region() == map_buffer.region())
			{
				this->vetoes_ << map_pool[j];
				break;
			}
		}
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

void User::update(const protobufs::UserStats &stats)
{
	update(stats, eros_->mapPool(), eros_->divisions());
}

void User::update(const protobufs::MapPool &vetoes, const QList<Map*> &map_pool)
{
	this->vetoes_.clear();

	for (int i = 0; i < vetoes.map_size(); i++)
	{
		const protobufs::Map &map_buffer = vetoes.map(i);
		for (int j = 0; j < map_pool.size(); j++)
		{
			if (map_pool[j]->battleNetId() == map_buffer.battle_net_id() && map_pool[j]->region() == map_buffer.region())
			{
				this->vetoes_ << map_pool[j];
				break;
			}
		}
	}

	emit updated(this);	
}

void User::update(const protobufs::MapPool &vetoes)
{
	update(vetoes, eros_->mapPool());
}