#ifndef LIBEROS_LOCALUSER_H
#define LIBEROS_LOCALUSER_H

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

private slots:
	void characterAdded(Character *character);
	void characterRemoved(Character *character);

private:
	
	QList<Character *> characters_;
};

#endif // LIBEROS_LOCALUSER_H
