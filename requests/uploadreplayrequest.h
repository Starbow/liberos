#ifndef UPLOADREPLAYREQUEST_H
#define UPLOADREPLAYREQUEST_H

#include "../request.h"

class UploadReplayRequest : public Request
{
	Q_OBJECT

public:
	UploadReplayRequest(Eros *parent, QIODevice *device);
	~UploadReplayRequest();

	bool processData(const QString &command, const QByteArray &data);
	bool processError(int code, const QByteArray &data);

	ErosError error() const;

private:
	ErosError error_;

};

#endif // UPLOADREPLAYREQUEST_H
