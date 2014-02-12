#include "pingrequest.h"
#include <QBuffer>

PingRequest::PingRequest(Eros *parent, const QByteArray &challenge)
	: Request(parent, "PNR")
{
	data_ = new QBuffer();
	data_->open(QIODevice::ReadWrite);
	data_->write(challenge);
	data_->close();
	data_->open(QIODevice::ReadOnly);

	ping_timer_ = new QTime();
	latency_ = 0;

}

PingRequest::~PingRequest()
{
	this->data_->close();
	delete this->data_;
	this->data_ = nullptr;
}


void PingRequest::requestSent()
{
	ping_timer_->restart();
}

bool PingRequest::processData(const QString &command, const QByteArray &data)
{
	if (command == "PNR") 
	{
		latency_ = ping_timer_->elapsed();
		emit complete(this);
		return true;
	}

	emit unhandled(this, command);
	return false;
}

int PingRequest::latency() const
{
	return this->latency_;
}