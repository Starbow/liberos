#include "map.h"

Map::Map(QObject *parent, ErosRegion region, int battle_net_id, const QString &name, const QString &description, const QString &info_url, const QString &preview_url)
	: QObject(parent), region_(region), battle_net_id_(battle_net_id), name_(name), description_(description), info_url_(info_url), preview_url_(preview_url)
{
	
}

Map::~Map()
{

}

const QString &Map::name() const
{
	return this->name_;
}
ErosRegion Map::region() const
{
	return this->region_;
}
int Map::battleNetId() const
{
	return this->battle_net_id_;
}
const QString &Map::description() const
{
	return this->description_;
}
const QString &Map::infoUrl() const
{
	return this->info_url_;
}
const QString &Map::previewUrl() const
{
	return this->preview_url_;
}