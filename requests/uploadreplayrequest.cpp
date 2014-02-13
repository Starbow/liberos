#include "uploadreplayrequest.h"

#include "../eros.pb.h"

UploadReplayRequest::UploadReplayRequest(Eros *parent, QIODevice *device)
	: Request(parent, "REP")
{
	this->data_ = device;
	this->error_ = ErosError::None;
}

UploadReplayRequest::~UploadReplayRequest()
{
	this->data_->close();
}

ErosError UploadReplayRequest::error() const
{
	return this->error_;
}

bool UploadReplayRequest::processData(const QString &command, const QByteArray &data)
{
	if (command == "REP")
	{
		protobufs::MatchResult result;
		result.ParseFromArray(data.data(), data.size());

		emit complete(this);
		return true;
	}

	return false;
}

bool UploadReplayRequest::processError(int code, const QByteArray &data)
{
	this->error_ = (ErosError)code;
	return true;
}