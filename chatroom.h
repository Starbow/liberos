#ifndef CHATROOM_H
#define CHATROOM_H

#include <QObject>
#include <QList>

class User;
class Eros;

namespace protobufs
{
	class ChatRoomInfo;
}

class ChatRoom : public QObject
{
	Q_OBJECT

public:
	ChatRoom(Eros *parent, const QString &name);
	~ChatRoom();

	bool joined() const;
	bool joinable() const;
	bool fixed() const;
	bool passworded() const;

	const QString &name() const;
	const QString &key() const;

	const QList<User *> &participants() const;


	void update(const protobufs::ChatRoomInfo &info);

	static const QString &cleanChatRoomName(const QString &name);
private:
	QString name_;
	QString key_;
	QList<User *> participants_;
	bool joinable_;
	bool fixed_;
	bool passworded_;
	bool joined_;
	int user_count_;
	Eros *eros_;

public slots:
	void join();
	void join(const QString &password);
	void leave();
	void sendMessage(const QString &message);

private slots:
	void userJoined(ChatRoom *room, User *user);
	void userLeft(ChatRoom *room, User *user);

};

#endif // CHATROOM_H
