#ifndef LIBEROS_REQUESTS_UPDATECHARACTERREQUEST_H
#define LIBEROS_REQUESTS_UPDATECHARACTERREQUEST_H

#include "../request.h"

class UpdateCharacterRequest : public Request
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
		VerificationFailed = 204,
	};

	UpdateCharacterRequest(Eros *parent, Character *character, int new_character_code, const QString new_game_profile_link);
	~UpdateCharacterRequest();

	bool processData(const QString &command, const QByteArray &data);
	bool processError(int code, const QByteArray &data);

	RequestStatus status() const;
	Character *character() const;
private:
	RequestStatus status_;
	Character *character_;
};

#endif // LIBEROS_REQUESTS_UPDATECHARACTERREQUEST_H
