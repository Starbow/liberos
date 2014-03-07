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
	this->active_regions_ = QList<ErosRegion>();
	this->map_pool_ = QList<Map*>();
	this->max_vetoes_ = 0;
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

const QList<ErosRegion> &HandshakeRequest::activeRegions() const
{
	return this->active_regions_;
}

const QList<Map*> &HandshakeRequest::mapPool() const
{
	return this->map_pool_;
}

bool HandshakeRequest::processHandshakeResponse(const QString &command, const QByteArray &data)
{
	protobufs::HandshakeResponse response;
    response.ParseFromArray(data.data(), data.size());
	
	if (response.has_status())
	{
		this->status_ = (ResponseStatus)response.status();

		if (this->status_ == ResponseStatus::Success)
		{
			if (response.has_max_vetoes())
			{
				this->max_vetoes_ = response.max_vetoes();
			}
			if (response.has_map_pool())
			{
				for (int i = 0; i < response.map_pool().map_size(); i++)
				{
					const protobufs::Map &map_buffer = response.map_pool().map(i);
					this->map_pool_ << new Map(this->parent(), (ErosRegion)map_buffer.region(), map_buffer.battle_net_id(), QString::fromStdString(map_buffer.battle_net_name()), QString::fromStdString(map_buffer.description()), QString::fromStdString(map_buffer.info_url()), QString::fromStdString(map_buffer.preview_url()));
				}
			}
			if (response.has_user())
			{
				this->user_ = new LocalUser(this->eros_, response, this->map_pool_);
				this->divisions_ = new Divisions(this->parent(), response);
				for (int i = 0; i < response.active_region_size(); i++)
				{
					this->active_regions_ << (ErosRegion)response.active_region(i);
				
				}
			}
			else
			{
				this->status_ = ResponseStatus::Fail;
			}
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

int HandshakeRequest::maxVetoes() const {
	return this->max_vetoes_;
}