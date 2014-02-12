#ifndef CHATLEAVEREQUEST_H
#define CHATLEAVEREQUEST_H

#include "../request.h"

class ChatLeaveRequest : public Request
{
	Q_OBJECT

public:
	ChatLeaveRequest(Eros *parent, ChatRoom *room);
	~ChatLeaveRequest();

	bool processData(const QString &command, const QByteArray &data);

	ChatRoom *room() const;
private:
	ChatRoom *room_;
};

#endif // CHATLEAVEREQUEST_H
