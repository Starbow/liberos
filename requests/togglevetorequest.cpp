#include "togglevetorequest.h"
#include "../eros.pb.h"

ToggleVetoRequest::ToggleVetoRequest(Eros *parent, Map *map)
	: Request(parent, "VET")
{
	protobufs::Map message;
	message.set_region((protobufs::Region)map->region());
	message.set_battle_net_id(map->battleNetId());
	message.set_battle_net_name(map->name().toStdString());

	this->data_ = serializeMessageToBuffer(&message);
	this->data_->open(QIODevice::ReadOnly);

	this->map_ = map;
	this->error_ = ErosError::None;
}

ToggleVetoRequest::~ToggleVetoRequest()
{
	this->data_->close();
	delete this->data_;
	this->data_ = nullptr;
}

bool ToggleVetoRequest::processData(const QString &command, const QByteArray &data)
{
	if (command == "VET")
	{
		protobufs::MapPool updated_vetoes;
		updated_vetoes.ParseFromArray(data.data(), data.size());
		eros_->localUser()->update(updated_vetoes);
		emit complete(this);
		return true;
	}

	return false;
}

bool ToggleVetoRequest::processError(int code, const QByteArray &data)
{
	this->error_ = (ErosError)code;
	emit complete(this);
	return true;
}

ErosError ToggleVetoRequest::error() const
{
	return this->error_;
}

Map *ToggleVetoRequest::map() const
{
	return this->map_;
}