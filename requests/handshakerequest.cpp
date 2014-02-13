#include "handshakerequest.h"
#include "../eros.pb.h"

HandshakeRequest::HandshakeRequest(Eros *parent, const QString &username, const QString &password)
	: Request(parent, "HSH")
{
	// Initialize and store the handshake request.
	protobufs::Handshake handshake;
	handshake.set_username(username.toStdString());
	handshake.set_auth_key(password.toStdString());
	
	this->data_ = serializeMessageToBuffer(&handshake);
	this->data_->open(QIODevice::ReadOnly);

	this->user_ = nullptr;
	this->status_ = ResponseStatus::NotReady; 
}

HandshakeRequest::~HandshakeRequest()
{
	this->data_->close();
	delete this->data_;
	this->data_ = nullptr;
}



HandshakeRequest::ResponseStatus HandshakeRequest::status() const 
{
	return this->status_;
}

LocalUser* HandshakeRequest::user() const
{
	return this->user_;  
}

Divisions* HandshakeRequest::divisions() const
{
	return this->divisions_;  
}


bool HandshakeRequest::processHandshakeResponse(const QString &command, const QByteArray &data)
{
	protobufs::HandshakeResponse response;
    response.ParseFromArray(data.data(), data.size());
	
	if (response.has_status() && response.has_user())
	{
		this->status_ = (ResponseStatus)response.status();

		if (this->status_ == ResponseStatus::Success)
		{
			this->user_ = new LocalUser(this->eros_, response);
			this->divisions_ = new Divisions(this->parent(), response);
		}
	} else {
		this->status_ = ResponseStatus::Fail;
	}
    

	emit complete(this);
	return true;
}

bool HandshakeRequest::processData(const QString &command, const QByteArray &data)
{
	if (command == "HSH") 
	{
		return this->processHandshakeResponse(command, data);
	}

	emit unhandled(this, command);
	return false;
}