#include "longprocessresponserequest.h"

LongProcessResponseRequest::LongProcessResponseRequest(Eros *parent, bool response)
	: Request(parent, "LPR")
{
	QByteArray data(1, (response ? '1' : '0'));
	data_ = new QBuffer();
	data_->open(QIODevice::ReadWrite);
	data_->write(data);
	data_->close();
	data_->open(QIODevice::ReadOnly);
}

LongProcessResponseRequest::~LongProcessResponseRequest()
{
	delete data_;
	this->data_ = nullptr;
}


bool LongProcessResponseRequest::processData(const QString &command, const QByteArray &data)
{
	if (command == "RLP")
	{
		this->error_ = ErosError::None;
		emit complete(this);
		return true;
	}

	return false;
}

bool LongProcessResponseRequest::processError(int code, const QByteArray &data)
{
	this->error_ = (ErosError)code;
	emit complete(this);
	return true;
}

ErosError LongProcessResponseRequest::error() const
{
	return this->error_;
}