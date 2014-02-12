#include "eros.h"
#include "eros.pb.h"
#include "character.h"


Character::Character(Eros *parent, ErosRegion region, int realm, int profile_id, const QString &display_name)
	: QObject(parent)
{
	this->region_ = region;
	this->realm_ = realm;
	this->profile_id_ = profile_id;
	this->display_name_ = display_name;
	this->verified_ = false;
	this->first_update_ = true;
}

Character::~Character()
{

}

void Character::setLinks()
{

}


void Character::update(const protobufs::Character &character)
{
	bool changed = false;
	if (character.has_character_code())
	{
		if (character.character_code() != this->character_code_)
		{
			this->character_code_ = character.character_code();
			changed = true;
		}
	}

	if (character.has_verified())
	{
		if (character.verified() != this->verified_)
		{
			this->verified_ = character.verified();
			changed = true;
		}
	}

	if (character.has_verification_portrait())
	{
		if (character.verification_portrait() != this->verification_portrait_)
		{
			this->verification_portrait_ = character.verification_portrait();
			changed = true;
		}
	}

	if (character.has_character_code())
	{
		if (character.character_code() != this->character_code_)
		{
			this->character_code_ = character.character_code();
			changed = true;
		}
	}

	if (character.has_character_name())
	{
		QString name = QString::fromStdString(character.character_name());
		if (name != this->display_name_)
		{
			this->display_name_ = name;
			changed = true;
		}
	}

	setLinks();

	if (this->first_update_)
	{
		this->first_update_ = false;
	} 
	else
	{
		if (changed)
		{
			emit updated(this);
		}
	}
}

ErosRegion Character::region() const
{
	return this->region_;
}
int Character::realm() const
{
	return this->realm_;
}
int Character::profile_id() const
{
	return this->profile_id_;
}
const QString &Character::display_name() const
{
	return this->display_name_;
}
int Character::character_code() const
{
	return this->character_code_;
}
const QString &Character::game_profile_link() const
{
	return this->game_profile_link_;
}
const QString &Character::web_profile_link() const
{
	return this->web_profile_link_;
}
const QString &Character::api_profile_link() const
{
	return this->api_profile_link_;
}
bool Character::verified() const
{
	return this->verified_;
}
int Character::verification_portrait() const
{
	return this->verification_portrait_;
}