#ifndef PINGREQUEST_H
#define PINGREQUEST_H

#include "../request.h"
#include <QTime>

class PingRequest : public Request
{
	Q_OBJECT

public:
	PingRequest(Eros *parent, const QByteArray &challenge);
	~PingRequest();

	bool processData(const QString &command, const QByteArray &data);

	int latency() const;
private slots:
		void requestSent();
private:
	QTime *ping_timer_;
	int latency_;
};

#endif // PINGREQUEST_H
