#include "localuser.h"
#include "eros.h"
#include "eros.pb.h"

LocalUser::LocalUser(Eros *parent, const protobufs::HandshakeResponse& handshake)
	: User(parent)
{
	this->characters_ = QList<Character *>();
	if (handshake.status() == protobufs::HandshakeResponse_HandshakeStatus_SUCCESS)
	{
		const protobufs::UserStats& user = handshake.user();

		this->username_ = QString::fromStdString(user.username());
		update(user);

		int i = 0;
		for (int i=0; i < handshake.character_size(); i++)
		{
			const protobufs::Character &character_message = handshake.character(i);
			QString display_name = QString::fromStdString(character_message.character_name());

			Character *character = new Character(parent, (ErosRegion)character_message.region(), character_message.subregion(), character_message.profile_id(), display_name);
			QObject::connect(character, SIGNAL(updated(Character*)), parent, SLOT(characterUpdatedHandler(Character*)));
			this->characters_ << character;
		}
	}
}

LocalUser::~LocalUser()
{

}
