#include "chatroom.h"
#include "eros.h"
#include "eros.pb.h"

ChatRoom::ChatRoom(Eros *parent, const QString &name)
	: QObject(0)
{
	this->name_ = name;
	this->eros_ = parent;
	this->key_ = name.trimmed().toLower();
	this->participants_ = QList<User *>();
	this->joinable_ = true;
	this->user_count_ = 0;
	this->fixed_ = false;
	this->joined_ = false;
	this->passworded_ = false;

	QObject::connect(parent, SIGNAL(chatRoomUserJoined(ChatRoom *, User *)), this, SLOT(userJoined(ChatRoom *, User *)));
	QObject::connect(parent, SIGNAL(chatRoomUserLeft(ChatRoom *, User *)), this, SLOT(userLeft(ChatRoom *, User *)));
}

ChatRoom::~ChatRoom()
{

}

void ChatRoom::update(const protobufs::ChatRoomInfo &info)
{
	this->fixed_ = info.fixed();
	this->joinable_ = info.joinable();
	this->key_ = QString::fromStdString(info.key());
	this->name_ = QString::fromStdString(info.name());
	this->user_count_ = info.users();
	this->passworded_ = info.passworded();

	if (info.participant_size() > 0)
	{
		this->participants_.clear();
	}

	for (int i = 0; i < info.participant_size(); i++)
	{
		const protobufs::UserStats &user_stats = info.participant(i);

		User* user = this->eros_->getUser(QString::fromStdString(user_stats.username()));
		user->update(user_stats);

		this->participants_ << user;
	}
}

const QList<User *> &ChatRoom::participants() const
{
	return this->participants_;
}

bool ChatRoom::joined() const
{
	return this->joined_;
}

bool ChatRoom::joinable() const
{
	return this->joinable_;
}

bool ChatRoom::fixed() const
{
	return this->fixed_;
}

bool ChatRoom::passworded() const
{
	return this->passworded_;
}

const QString &ChatRoom::name() const
{
	return this->name_;
}

const QString &ChatRoom::key() const
{
	return this->key_;
}

void ChatRoom::userJoined(ChatRoom *room, User *user)
{
	if (room == this)
	{
		if (!this->participants_.contains(user))
		{
			this->participants_ << user;
		}

		if (user == this->eros_->localUser())
		{
			joined_ = true;
		}
	}
}


void ChatRoom::userLeft(ChatRoom *room, User *user)
{
	if (room == this)
	{
		if (this->participants_.contains(user))
		{
			this->participants_.removeAll(user);
		}

		if (user == this->eros_->localUser())
		{
			joined_ = false;
		}
	}
}

void ChatRoom::join()
{
	this->eros_->joinChatRoom(this, ""); 
}
void ChatRoom::join(const QString &password)
{
	this->eros_->joinChatRoom(this, password);
}
void ChatRoom::leave()
{
	this->eros_->leaveChatRoom(this);
}
void ChatRoom::sendMessage(const QString &message)
{
	this->eros_->sendMessage(this, message);
}