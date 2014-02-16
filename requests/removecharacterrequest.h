#ifndef LIBEROS_REQUESTS_REMOVECHARACTERREQUEST_H
#define LIBEROS_REQUESTS_REMOVECHARACTERREQUEST_H

#include "../request.h"

class RemoveCharacterRequest : public Request
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
	};

	RemoveCharacterRequest(Eros *parent, Character *character);
	~RemoveCharacterRequest();

	bool processData(const QString &command, const QByteArray &data);
	bool processError(int code, const QByteArray &data);

	RequestStatus status() const;
	Character *character() const;
private:
	RequestStatus status_;
	Character *character_;
};

#endif // LIBEROS_REQUESTS_REMOVECHARACTERREQUEST_H
