#ifndef EROS_H
#define EROS_H

#include <QObject>
#include <QList>
#include <QTcpSocket>
#include <QTime>
#include <QMap>
#include <QAtomicInt>
#include <QBuffer>

#include "enums.h"
#include "user.h"
#include "localuser.h"
#include "character.h"
#include "chatroom.h"
#include "matchmakingmatch.h"
#include "divisions.h"



namespace google
{
	namespace protobuf
	{
		class Message;
	}
}

namespace protobufs
{
	class HandshakeResponse;
	class UserStats;
	class Handshake;
};

#define UPLOAD_BUFFER_SIZE 4096
#define DEFAULT_PORT 12345

class Request;

class Eros : public QObject
{
	Q_OBJECT

public:

	Eros(QObject *parent);
	~Eros();

	int lastError();
	QString errorString(int code) const;
	QString errorString() const;

	ErosState state() const;
	ErosMatchmakingState matchmakingState() const;

	int regionSearchingUserCount(ErosRegion region) const;
	int searchingUserCount() const;
	int activeUserCount() const;

	
	const Divisions *divisions() const;
	const MatchmakingMatch *matchmakingMatch() const;

	LocalUser *localUser();
	User *getUser(const QString &username);
	

	const QList<ChatRoom *> &chatRooms() const;
	ChatRoom *getChatRoom(const QString &room);
	


	static const QString regionToString(ErosRegion region);
	static const QString regionToLanguageString(ErosRegion region);
	static const QString regionToDomainString(ErosRegion region);

public slots:
	// Connectivity slots
	void connectToEros(const QString server, const QString username, const QString password);
	void disconnectFromEros();

	// Matchmaking slots
	void queueMatchmaking(ErosRegion region, int search_radius);
	void dequeueMatchmaking();

	// Chat slots
	void sendMessage(ChatRoom *room, const QString message);
	void sendMessage(User *user, const QString message);
	void joinChatRoom(ChatRoom *room);
	void joinChatRoom(ChatRoom *room, const QString password);
	void leaveChatRoom(ChatRoom *room);
	void refreshChatRooms();

	// Character slots
	void addCharacter(const QString battle_net_url);
	void updateCharacter(Character *character, int new_character_code, const QString new_game_profile_link);
	void removeCharacter(Character *character);

	//Upload replay slots
	void uploadReplay(const QString path);
	void uploadReplay(QIODevice *device);

private:
	QString server_hostname_;
	quint16 server_port_;
	QString username_;
	QString password_;

	QList<ChatRoom *> chatrooms_;
	QList<User *> users_;
	QMap<int, Request *> requests_;
	QTcpSocket *socket_;
	LocalUser *local_user_;
	MatchmakingMatch *matchmaking_match_;
	Divisions *divisions_;

	int last_error_;
	QAtomicInt transaction_id_base_;

	int latency_;

	ErosState state_;
	ErosMatchmakingState matchmaking_state_;

	// I don't even know if this is necessary, but it's better safe than sorry
	bool awaiting_data_;
	QString awaiting_data_command_;
	qint64 awaiting_data_size_;
	qint64 awaiting_data_transaction_id_;
	QBuffer *awaiting_data_buffer_;

	// Server Stats
	QMap<ErosRegion, int> eros_matchmaking_searching_regions_;
	int eros_active_user_count_;
	int eros_searching_user_count_;

	void setState(ErosState);
	void setStats(int active, int searching);
	void setRegionStats(ErosRegion region, int searching);

	void sendRequest(Request *request);

	void handleCommand(const QString &command, const int &transaction_id, const QByteArray &data);
	void handleServerCommand(const QString &command, const QByteArray &data);

	void handleUserStats(const protobufs::UserStats &stats);

	void setMatchmakingState(ErosMatchmakingState);
private slots:
	void socketConnected();
    void socketDisconnected();
    void socketReadyRead();
	void socketError(QAbstractSocket::SocketError socketError);
	void requestComplete(Request *);
	void requestError(Request *, int);
	
	void userUpdatedHandler(User *user);
	void characterUpdatedHandler(Character* character);

	void handshakeRequestComplete(Request *);
	void matchmakingRequestQueued(Request *);
	void matchmakingRequestComplete(Request *);
	
	void chatIndexRequestComplete(Request *);
	void chatJoinRequestComplete(Request *);
	void chatLeaveRequestComplete(Request *);
	void chatMessageRequestComplete(Request *);

	void characterRequestComplete(Request *);

    void replayRequestComplete(Request *request);

	void ensureUserParent(User *user);
	void ensureChatParent(ChatRoom *room);
signals:

	// Low level connection signals.
	void stateChanged(ErosState);
	void connectionError(QAbstractSocket::SocketError);

	// Basic operation signals
	void connected();
	void disconnected();
	void handshakeFailed();
	void serverError(int);
	void statsUpdated(int active, int searching);
	void regionStatsUpdated(ErosRegion region, int searching);
	void uploadProgress(qint64 written, qint64 total);

	// User signals
	void userAdded(User *user);
	void userUpdated(User *user);
	void localUserUpdated(LocalUser *user);
	void characterAdded(Character* character);
	void characterUpdated(Character* character);
	void characterRemoved(Character* character);
	void addCharacterError(const QString battle_net_profile, ErosError error);
	void updateCharacterError(Character* character, ErosError error);
	void removeCharacterError(Character* character, ErosError error);

	// Match signals
	void matchmakingStateChanged(ErosMatchmakingState status);
	void matchmakingMatchFound(MatchmakingMatch *match);
	void replayUploaded();
	void replayUploadError(ErosError error);

	// Chat signals
	void chatRoomJoinFailed(ChatRoom *room, ErosError code);

	void chatRoomAdded(ChatRoom *room);
	void chatRoomRemoved(ChatRoom *room);
	void chatRoomJoined(ChatRoom *room);
	void chatRoomLeft(ChatRoom *room);

	void chatRoomUserJoined(ChatRoom *room, User *user);
	void chatRoomUserLeft(ChatRoom *room, User *user);
	
	void chatMessageReceieved(ChatRoom *room, User *user, const QString message);
	void chatMessageReceieved(User *user, const QString message);

	void chatMessageSent(ChatRoom *room, const QString message);
	void chatMessageSent(User *user, const QString message);
	void chatMessageFailed(ChatRoom *room, const QString message, ErosError code);
	void chatMessageFailed(User *user, const QString message, ErosError code);
	

};

#endif // EROS_H
