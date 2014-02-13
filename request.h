#ifndef REQUEST_H
#define REQUEST_H

#include <QObject>
#include <QBuffer>

#include "eros.h"


class Request : public QObject
{
	Q_OBJECT

public:
	Request(Eros *parent, const QString &command);
	~Request();

	virtual bool processData(const QString &command, const QByteArray &data) = 0;
	virtual bool processError(int code, const QByteArray &data);

	const QString &command() const;
	QIODevice* data() const;

	static QBuffer *serializeMessageToBuffer(::google::protobuf::Message *);

private slots:
	virtual void requestSent();
signals:
	void complete(Request * request);
	void error(Request * request, int code);
	void unhandled(Request * Request, const QString& command);

protected:
	QString command_;
	QIODevice *data_;
	Eros *eros_;
};

#endif // REQUEST_H
