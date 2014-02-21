#include "longprocessrequest.h"

LongProcessRequest::LongProcessRequest(Eros *parent, int process)
	: Request(parent, "RLP")
{
	QByteArray data(1, (char)process);
	data_ = new QBuffer();
	data_->open(QIODevice::ReadWrite);
	data_->write(data);
	data_->close();
	data_->open(QIODevice::ReadOnly);
}

LongProcessRequest::~LongProcessRequest()
{
	delete data_;
	this->data_ = nullptr;
}


bool LongProcessRequest::processData(const QString &command, const QByteArray &data)
{
	if (command == "RLP")
	{
		this->error_ = ErosError::None;
		emit complete(this);
		return true;
	}
}

bool LongProcessRequest::processError(int code, const QByteArray &data)
{
	this->error_ = (ErosError)code;
	emit complete(this);
	return true;
}

ErosError LongProcessRequest::error() const
{
	return this->error_;
}