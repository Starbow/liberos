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
int User::wins() const
{
	return this->wins_;
}
int User::losses() const
{
	return this->losses_;
}
int User::forefeits() const
{
	return this->forefeits_;
}
int User::walkovers() const
{
	return this->walkovers_;
}
int User::searchRadius() const
{
	return this->search_radius_;
}

int User::points() const
{
	return this->points_;
}

void User::update(const protobufs::UserStats &stats)
{
	this->state_ = ErosUserState::Known;
	this->username_ = QString::fromStdString(stats.username());
	this->wins_ = stats.wins();
	this->losses_ = stats.losses();
	this->walkovers_ = stats.walkovers();
	this->forefeits_ = stats.forefeits();
	this->points_ = stats.points();
	this->search_radius_ = stats.search_radius();
	
	if (this->first_update_)
	{
		this->first_update_ = false;
	} 
	else
	{
		emit updated(this);
	}	
}