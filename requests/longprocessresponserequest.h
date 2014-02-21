#ifndef LIBEROS_LONGPROCESSRESPONSEREQUEST_H
#define LIBEROS_LONGPROCESSRESPONSEREQUEST_H

#include "../request.h"

class LongProcessResponseRequest : public Request
{
	Q_OBJECT

public:
	LongProcessResponseRequest(Eros *parent, bool response);
	~LongProcessResponseRequest();

	bool processData(const QString &command, const QByteArray &data);
	bool processError(int code, const QByteArray &data);

	ErosError error() const;
private:
	ErosError error_;
};

#endif // LIBEROS_LONGPROCESSRESPONSEREQUEST_H
