#include "request.h"
#include "eros.pb.h"

Request::Request(Eros *parent, const QString &command)
	: QObject(parent)
{
	this->command_ = command;
	this->eros_ = parent;
	this->data_ = nullptr;
}

Request::~Request()
{
	
}

void Request::requestSent()
{

}

bool Request::processError(int code, const QByteArray &data)
{
	emit error(this, code);
	return true;
}


QBuffer *Request::serializeMessageToBuffer(::google::protobuf::Message *message)
{
	QByteArray data;
	data.resize(message->ByteSize());

	if (message->SerializeToArray(data.data(), data.size()))
	{
		QBuffer *buffer = new QBuffer();
		buffer->open(QIODevice::ReadWrite);
		buffer->write(data);
		buffer->seek(0);
		buffer->close();
		return buffer;
	} else {
		return nullptr;
	}

}

const QString &Request::command() const 
{
	return this->command_;
}

QIODevice* Request::data() const
{
	return this->data_;
}