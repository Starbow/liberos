#ifndef LOCALUSER_H
#define LOCALUSER_H

#include <QObject>

#include "user.h"
#include "character.h"

class Eros;

class LocalUser : public User
{
	Q_OBJECT

public:
	LocalUser(Eros *parent, const protobufs::HandshakeResponse& handshake);
	~LocalUser();

	const QList<Character *> &characters() const;
private:
	
	QList<Character *> characters_;
};

#endif // LOCALUSER_H
