#ifndef LIBEROS_LONGPROCESSREQUEST_H
#define LIBEROS_LONGPROCESSREQUEST_H

#include "../request.h"

class LongProcessRequest : public Request
{
	Q_OBJECT

public:
	LongProcessRequest(Eros *parent, int process);
	~LongProcessRequest();

	bool processData(const QString &command, const QByteArray &data);
	bool processError(int code, const QByteArray &data);

	ErosError error() const;
private:
	ErosError error_;
};

#endif // LIBEROS_LONGPROCESSREQUEST_H
