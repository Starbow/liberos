#ifndef LIBEROS_REQUESTS_TOGGLEVETOREQUEST_H
#define LIBEROS_REQUESTS_TOGGLEVETOREQUEST_H

#include "../request.h"

class ToggleVetoRequest : public Request
{
	Q_OBJECT

public:
	ToggleVetoRequest(Eros *parent, Map *map);
	~ToggleVetoRequest();

	bool processData(const QString &command, const QByteArray &data);
	bool processError(int code, const QByteArray &data);

	ErosError error() const;
	Map *map() const;
private:
	ErosError error_;
	Map *map_;
};

#endif // LIBEROS_REQUESTS_TOGGLEVETOREQUEST_H
