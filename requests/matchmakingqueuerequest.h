#ifndef LIBEROS_REQUESTS_MATCHMAKINGQUEUEREQUEST_H
#define LIBEROS_REQUESTS_MATCHMAKINGQUEUEREQUEST_H

#include "../request.h"

class MatchmakingMatch;

class MatchmakingQueueRequest : public Request
{
	Q_OBJECT

public:
	MatchmakingQueueRequest(Eros *parent, ErosRegion region, int search_radius);
	~MatchmakingQueueRequest();

	bool processData(const QString &command, const QByteArray &data);
	bool processError(int code, const QByteArray &data);

	ErosMatchmakingState status() const;
	MatchmakingMatch *match() const;

private:
	ErosMatchmakingState status_;
	MatchmakingMatch *match_;
	QTime *matchmaking_timer_;

private slots:
	void requestSent();
signals:
	void queued(Request *);
};

#endif // LIBEROS_REQUESTS_MATCHMAKINGQUEUEREQUEST_H
