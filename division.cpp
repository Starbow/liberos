#include "division.h"
#include "eros.pb.h"

Division::Division(QObject *parent, const protobufs::Division &division)
	: QObject(parent)
{
	this->id_ = division.id();
	this->name_ = QString::fromStdString(division.name());
	this->small_icon_url_ = QString::fromStdString(division.small_icon_url());
	this->icon_url_ = QString::fromStdString(division.icon_url());
	this->promotion_threshold_ = division.promotion_threshold() * 100;
	this->demotion_threshold_ = division.demotion_threshold() * 100;
}

Division::~Division()
{

}

Division::Division(QObject *parent)
	: QObject(parent)
{
	this->id_ = 0;
	this->name_ = tr("Placement");
	this->promotion_threshold_ = 0;
	this->demotion_threshold_ = 0;
}

int Division::id() const
{
	return this->id_;
}

const QString &Division::name() const
{
	return this->name_;
}

int Division::promotionThreshold() const
{
	return this->promotion_threshold_;
}
int Division::demotionThreshold() const
{
	return this->demotion_threshold_;
}
const QString &Division::iconUrl() const
{
	return this->icon_url_;
}
const QString &Division::smallIconUrl() const
{
	return this->small_icon_url_;
}