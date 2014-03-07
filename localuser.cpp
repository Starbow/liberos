#include "localuser.h"
#include "eros.h"
#include "eros.pb.h"

LocalUser::LocalUser(Eros *parent, const protobufs::HandshakeResponse& handshake, const QList<Map*> &map_pool)
	: User(parent)
{
	this->characters_ = QList<Character *>();
	if (handshake.status() == protobufs::HandshakeResponse_HandshakeStatus_SUCCESS)
	{
		const protobufs::UserStats& user = handshake.user();

		this->username_ = QString::fromStdString(user.username());
		update(user, map_pool);

		for (int i=0; i < handshake.character_size(); i++)
		{
			const protobufs::Character &character_message = handshake.character(i);
			QString display_name = QString::fromStdString(character_message.character_name());

			Character *character = new Character(parent, character_message);
			QObject::connect(character, SIGNAL(updated(Character*)), this->eros_, SLOT(characterUpdatedHandler(Character*)));
			this->characters_ << character;
		}
	}

	QObject::connect(this->eros_, SIGNAL(characterAdded(Character*)), this, SLOT(characterAdded(Character*)));
	QObject::connect(this->eros_, SIGNAL(characterRemoved(Character*)), this, SLOT(characterRemoved(Character*)));
}

LocalUser::~LocalUser()
{

}

void LocalUser::characterAdded(Character *character)
{
	QObject::connect(character, SIGNAL(updated(Character*)), this->eros_, SLOT(characterUpdatedHandler(Character*)));
	this->characters_ << character;
}

void LocalUser::characterRemoved(Character *character)
{
	this->characters_.removeAll(character);
}

const QList<Character *> &LocalUser::characters() const
{
	return this->characters_;
}
