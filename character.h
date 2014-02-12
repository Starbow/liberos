#ifndef CHARACTER_H
#define CHARACTER_H

#include <QObject>
#include "enums.h"

namespace protobufs
{
	class Character;
}
class Eros;


class Character : public QObject
{
	Q_OBJECT

public:
	Character(Eros *parent, ErosRegion region, int realm, int character_id, const QString &display_name);
	~Character();

	ErosRegion region() const;
	int realm() const;
	int profile_id() const;
	const QString &display_name() const;
	int character_code() const;
	const QString &game_profile_link() const;
	const QString &web_profile_link() const;
	const QString &api_profile_link() const;
	bool verified() const;
	int verification_portrait() const;

	void update(const protobufs::Character &character);

signals:
	void updated(Character *character);

private:
	ErosRegion region_;
	int realm_;
	int profile_id_;
	QString display_name_;
	int character_code_;
	QString game_profile_link_;
	QString web_profile_link_;
	QString api_profile_link_;
	bool verified_;
	int verification_portrait_;

	bool first_update_;

	void setLinks();
};

#endif // CHARACTER_H
