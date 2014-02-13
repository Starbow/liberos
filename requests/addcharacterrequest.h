#ifndef ADDCHARACTERREQUEST_H
#define ADDCHARACTERREQUEST_H

#include <QObject>
#include "../request.h"

class AddCharacterRequest : public Request
{
	Q_OBJECT

public:
	enum RequestStatus
	{
		NotReady = -1,
		Success = 0,
		DataBaseReadError = 101,
		DataBaseWriteError = 102,
		BadRequest = 201,
		CharacterAlreadyExists = 202,
		BattleNetCommunicationError = 203,
	};

	AddCharacterRequest(Eros *parent, const QString &battle_net_url);
	~AddCharacterRequest();

	bool processData(const QString &command, const QByteArray &data);
	bool processError(int code, const QByteArray &data);

	RequestStatus status() const;
	Character *character() const;
	const QString &battle_net_url() const;
private:
	
	RequestStatus status_;
	Character *character_;
	QString battle_net_url_;
};

#endif // ADDCHARACTERREQUEST_H
