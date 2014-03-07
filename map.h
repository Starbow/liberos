#ifndef MAP_H
#define MAP_H

#include <QObject>
#include "enums.h"

namespace protobufs 
{
	class Map;
}

class Map : public QObject
{
	Q_OBJECT

public:
	Map(QObject *parent, ErosRegion region, int battle_net_id, const QString &name, const QString &description, const QString &info_url, const QString &preview_url);
	~Map();

	const QString &name() const;
	ErosRegion region() const;
	int battleNetId() const;
	const QString &description() const;
	const QString &infoUrl() const;
	const QString &previewUrl() const;
private:
	QString name_;
	ErosRegion region_;
	int battle_net_id_;
	QString description_;
	QString info_url_;
	QString preview_url_;
};

#endif // MAP_H
