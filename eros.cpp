#include "eros.h"
#include "eros.pb.h"
#include "request.h"

#include "requests/handshakerequest.h"
#include "requests/pingrequest.h"
#include "requests/matchmakingdequeuerequest.h"
#include "requests/matchmakingqueuerequest.h"
#include "requests/chatindexrequest.h"
#include "requests/chatjoinrequest.h"
#include "requests/chatleaverequest.h"
#include "requests/chatmessagerequest.h"
#include "requests/privatemessagerequest.h"
#include "requests/addcharacterrequest.h"
#include "requests/updatecharacterrequest.h"
#include "requests/removecharacterrequest.h"
#include "requests/uploadreplayrequest.h"

#include <QTimer>
#include <QThread>
#include <QFile>

Eros::Eros(QObject *parent)
	: QObject(parent)
{
	qRegisterMetaType<ErosRegion>();
	qRegisterMetaType<ErosError>();

	this->users_ = QList<User *>();
	this->chatrooms_ = QList<ChatRoom *>();
	this->last_error_ = 0;
	this->socket_ = new QTcpSocket(this);
	this->state_ = ErosState::UnconnectedState;
	this->awaiting_data_ = false;

	QObject::connect(this->socket_, SIGNAL(connected()), this, SLOT(socketConnected()));
    QObject::connect(this->socket_, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
    QObject::connect(this->socket_, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
	QObject::connect(this->socket_, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));

	QObject::connect(this, SIGNAL(userAdded(User*)), this, SLOT(ensureUserParent(User*)));
	QObject::connect(this, SIGNAL(chatRoomAdded(ChatRoom*)), this, SLOT(ensureChatParent(ChatRoom*)));
}


Eros::~Eros()
{
	delete this->socket_;
}

// Ugly cross-threading hack.
void Eros::ensureUserParent(User *user)
{
	user->setParent(this);
}

void Eros::ensureChatParent(ChatRoom *room)
{
	room->setParent(this);
}

const QString Eros::regionToString(ErosRegion region)
{
	switch (region)
	{
		case ErosRegion::NA:
			return tr("NA");
			break;
		case ErosRegion::KR:
			return tr("KR");
			break;
		case ErosRegion::EU:
			return tr("EU");
			break;
		case ErosRegion::SEA:
			return tr("SEA");
			break;
		case ErosRegion::CN:
			return tr("CN");
			break;
		default:
			return tr("Unknown");
			break;
	}
}

void Eros::refreshChatRooms()
{
	ChatIndexRequest *request = new ChatIndexRequest(this);
	QObject::connect(request, SIGNAL(complete(Request*)), this, SLOT(chatIndexRequestComplete(Request*)));
	sendRequest(request);
}

const QString Eros::regionToDomainString(ErosRegion region)
{
	switch (region)
	{
		case ErosRegion::NA:
			return QString("us.battle.net");
			break;
		case ErosRegion::KR:
			return QString("kr.battle.net");
			break;
		case ErosRegion::EU:
			return QString("eu.battle.net");
			break;
		case ErosRegion::SEA:
			return QString("sea.battle.net");
			break;
		case ErosRegion::CN:
			return QString("www.battlenet.com.cn");
			break;
		default:
			return QString::null;
			break;
	}
}

const QString Eros::regionToLanguageString(ErosRegion region)
{
	switch (region)
	{
		case ErosRegion::NA:
		case ErosRegion::EU:
		case ErosRegion::SEA:
			return QString("en");
			break;
		case ErosRegion::CN:
			return QString("zh");
			break;
		case ErosRegion::KR:
			return QString("ko");
		default:
			return QString("en");
			break;
	}
}

void Eros::socketConnected()
{
	HandshakeRequest *request = new HandshakeRequest(this, this->username_, this->password_);
	QObject::connect(request, SIGNAL(complete(Request*)), this, SLOT(handshakeRequestComplete(Request*)));
	sendRequest(request);
}

void Eros::socketDisconnected() {
	disconnectFromEros();
}

void Eros::socketError(QAbstractSocket::SocketError error)
{
	emit connectionError(error);
	disconnectFromEros();
}

void Eros::socketReadyRead()
{
    QString line;


    //If we get an incomplete read, buffer the data we do have for the next read.
	if (!this->awaiting_data_)
	{
        while (this->socket_->canReadLine())
		{

			QString line = QString(this->socket_->readLine()).trimmed();
			//qDebug() << ">> " << line << endl;
            QStringList split = line.split(" ");

            
			if (split.size() == 3)
			{
				this->awaiting_data_transaction_id_ = split[1].toInt();
				this->awaiting_data_size_ = split[2].toInt();
			}
			else if (split.size() == 2)
			{
				this->awaiting_data_transaction_id_ = -1;
				this->awaiting_data_size_ = split[1].toInt();
			}

            QByteArray data = this->socket_->read(this->awaiting_data_size_);

            if (data.size() != this->awaiting_data_size_)
			{
                this->awaiting_data_ = true;
                this->awaiting_data_command_ = split[0];

                QBuffer buffer(this);
                this->awaiting_data_buffer_ = &buffer;
                this->awaiting_data_buffer_->open(QBuffer::ReadWrite);
                if (data.size() > 0)
				{
                    this->awaiting_data_buffer_->write(data);
                }
            }
			else
			{
				handleCommand(split[0], this->awaiting_data_transaction_id_, data);
            }
        }
    }
	else
	{
		QByteArray data = socket_->read(this->awaiting_data_size_ - this->awaiting_data_buffer_->size());
        if (data.size() == 0) {
            return;
        }

        awaiting_data_buffer_->write(data);

        //We have the full payload
        if (this->awaiting_data_buffer_->size() == this->awaiting_data_size_)
		{
            handleCommand(this->awaiting_data_command_, this->awaiting_data_transaction_id_, this->awaiting_data_buffer_->buffer());
			this->awaiting_data_ = false;
            this->awaiting_data_buffer_->close();
            this->awaiting_data_buffer_ = NULL;
        }
    }
}


// Gets the last server error code.
int Eros::lastError()
{
	return this->last_error_;
}

// Converts a server error code to a string.
QString Eros::errorString(int code) const
{
	switch (code)
	{
	case 0:
	default:
		return tr("Unknown error");
		break;
	}
}

// Converts the last server error to a string.
QString Eros::errorString() const 
{
	return this->errorString(this->last_error_);
}

void Eros::disconnectFromEros() 
{
	if (this->socket_->state() != QTcpSocket::SocketState::UnconnectedState)
	{
		setState(ErosState::DisconnectingState);
		this->socket_->abort();
	}
	this->local_user_ = nullptr;

	// Should probably ensure the items contained are cleaned up as well.
	this->users_.clear();
	this->chatrooms_.clear();
	this->requests_.clear();

	setState(ErosState::UnconnectedState);
}

// Connects to the Eros server.
void Eros::connectToEros(const QString server, const QString username, const QString password)
{
	this->transaction_id_base_ = QAtomicInt(0);

	QStringList tokens = server.split(":");

	// Check if the server contains a port and act accordingly.
	if (tokens.size() == 2)
	{
		this->server_hostname_ = tokens.at(0);
		this->server_port_ = tokens.at(1).toUInt();
		
	} else {
		this->server_hostname_ = server;
		this->server_port_ = DEFAULT_PORT;
	}

	disconnectFromEros();

	this->username_ = username;
	this->password_ = password;
	

	setState(ErosState::ConnectingState);
	this->socket_->connectToHost(this->server_hostname_, this->server_port_);

	
}

// Sets the state and emits the stateChanged signal.
void Eros::setState(ErosState state)
{
	if (this->state_ != state) 
	{
		this->state_ = state;
		emit stateChanged(this->state_);

		if (this->state_ == ErosState::UnconnectedState)
		{
			emit disconnected();
		} 
		else if (this->state_ == ErosState::ConnectedState)
		{
			emit connected();
		}
	}
}

// Sets the state and emits the stateChanged signal.
void Eros::setMatchmakingState(ErosMatchmakingState state)
{
	if (this->matchmaking_state_ != state) 
	{
		this->matchmaking_state_ = state;
		emit matchmakingStateChanged(this->matchmaking_state_);

		if (this->state_ == ErosState::UnconnectedState)
		{
			emit disconnected();
		} 
		else if (this->state_ == ErosState::ConnectedState)
		{
			emit connected();
		}
	}
}

const Divisions *Eros::divisions() const
{
	return this->divisions_;
}

const MatchmakingMatch *Eros::matchmakingMatch() const
{
	return this->matchmaking_match_;
}

User* Eros::getUser(const QString &username)
{
	const QString key = username.toLower();

	QList<User *>::iterator i;
	for (i = this->users_.begin(); i != this->users_.end(); ++i)
	{
		if ((*i)->username().toLower() == key)
		{
			return ((*i));
		}
	}

	// Ugly cross-thread hack.
	User *new_user = new User(this, username);
	if (QThread::currentThread() == this->thread())
	{
		new_user->setParent(this);
	} 
	else 
	{
		new_user->moveToThread(this->thread());
	}

	this->users_ << new_user;
	emit userAdded(new_user);
	return new_user;
}

ErosState Eros::state() const
{
	return this->state_;
}

void Eros::handleCommand(const QString &command, const int &transaction_id, const QByteArray &data)
{

	bool is_error = false;
    int error_code = 0;
    error_code = command.toInt(&is_error);
    Request* request = nullptr;

	if (transaction_id != -1)
	{
		if (this->requests_.contains(transaction_id))
		{
			request = this->requests_.value(transaction_id);
		}
	}

	if (is_error) 
	{
		emit serverError(error_code);
	}

	bool processed = false;
	if (request != nullptr)
	{
		if (is_error)
		{
			processed = request->processError(error_code, data);
			emit serverError(error_code);
		}
		else
		{
			processed = request->processData(command, data);
		}
	}


	if (!processed && request != nullptr)
	{
		qDebug() << "The Request instance assigned to the transaction failed to process the " << command << " command.";
	}
	
	if (!processed)
	{
		handleServerCommand(command, data);
	}
}

ChatRoom *Eros::getChatRoom(const QString &room)
{
	const QString key = room.trimmed().toLower();

	QList<ChatRoom *>::iterator i;
	for (i = this->chatrooms_.begin(); i != this->chatrooms_.end(); ++i)
	{
		if ((*i)->key() == key)
		{
			return ((*i));
		}
	}

	// Ugly cross-thread hack.
	ChatRoom *new_room = new ChatRoom(this, room);
	if (QThread::currentThread() == this->thread())
	{
		new_room->setParent(this);
	} 
	else 
	{
		new_room->moveToThread(this->thread());
	}
	return new_room;
}

void Eros::queueMatchmaking(ErosRegion region, int radius)
{
	if (state_ == ErosState::ConnectedState)
	{
		MatchmakingQueueRequest *request = new MatchmakingQueueRequest(this, region, radius);
		QObject::connect(request, SIGNAL(queued(Request*)), this, SLOT(matchmakingRequestQueued(Request*)));
		sendRequest(request);
	}
}

void Eros::dequeueMatchmaking()
{
	// Dequeue from matchmaking. Check locally to ensure we're not already queued.
	if (state_ == ErosState::ConnectedState)
	{
		if (matchmaking_state_ == ErosMatchmakingState::Queued)
		{
			MatchmakingDequeueRequest *request = new MatchmakingDequeueRequest(this);
			sendRequest(request);
		}
	}
}

void Eros::handleServerCommand(const QString &command, const QByteArray &data)
{
	if (command == "PNG") // Ping/Challenge
	{
		sendRequest(new PingRequest(this, data));		
	}
	else if (command == "SSU") // server stats update
	{
		protobufs::ServerStats stats;
		stats.ParseFromArray(data.data(), data.size());

		if (stats.has_active_users() && stats.has_searching_users())
		{
			setStats(stats.active_users(), stats.searching_users());
		}

		for (int i=0; i < stats.region_size(); i++)
		{
			const protobufs::MatchmakingStats &region= stats.region(i);
			if (region.has_region() && region.has_searching_users())
			{
				setRegionStats((ErosRegion)region.region(), region.searching_users());
			}
		}		
	}
	else if (command == "USU") // User stats update
	{
		protobufs::UserStats stats;
		stats.ParseFromArray(data.data(), data.size());

		if (stats.has_username()) {
			handleUserStats(stats);
		}
	}
	else if (command == "CHJ" || command == "CHL")
	{
		protobufs::ChatRoomUser message;
		message.ParseFromArray(data.data(), data.size());

		const protobufs::UserStats &user_message = message.user();
		const protobufs::ChatRoomInfo &room_message = message.room();
		
		handleUserStats(user_message);
		
		User *user = getUser(QString::fromStdString(user_message.username()));
		ChatRoom *room = getChatRoom(QString::fromStdString(room_message.name()));
				
		room->update(room_message);

		if (command == "CHJ")
		{
			emit chatRoomUserJoined(room, user);
			if (user == this->local_user_)
			{
				emit chatRoomJoined(room);
			}
		}
		else if (command == "CHL")
		{
			emit chatRoomUserLeft(room, user);
			if (user == this->local_user_)
			{
				emit chatRoomLeft(room);
			}
		}
	}
	else if (command == "CHM") // chat room message
	{
		protobufs::ChatRoomMessage message;
		message.ParseFromArray(data.data(), data.size());

		const protobufs::UserStats &user_message = message.sender();
		const protobufs::ChatRoomInfo &room_message = message.room();

		handleUserStats(user_message);
		User *user = getUser(QString::fromStdString(user_message.username()));
		ChatRoom *room = getChatRoom(QString::fromStdString(room_message.name()));

		emit chatMessageReceieved(room, user, QString::fromStdString(message.message()));
	}
	else if (command == "CHP") // Private message
	{
		protobufs::ChatPrivateMessage message;
		message.ParseFromArray(data.data(), data.size());

		const protobufs::UserStats &user_message = message.sender();

		handleUserStats(user_message);
		User *user = getUser(QString::fromStdString(user_message.username()));

		emit chatMessageReceieved(user, QString::fromStdString(message.message()));
	}
}
void Eros::sendRequest(Request *request)
{
	if (this->socket_->state() != QTcpSocket::SocketState::ConnectedState)
	{
		disconnectFromEros();
		return;
	}
	// This should be in a worker thread.

	int transaction_id = transaction_id_base_.fetchAndAddRelaxed(1);

	
	QObject::connect(request, SIGNAL(complete(Request*)), this, SLOT(requestComplete(Request*)));
	QObject::connect(request, SIGNAL(error(Request*, int)), this, SLOT(requestError(Request*, int)));


	requests_.insert(transaction_id, request);

	qint64 dataLength = 0;

	QIODevice *data = request->data();
	char* buffer = nullptr;
	if (data != nullptr)
	{
		dataLength = data->size();
		buffer = (char*)malloc(UPLOAD_BUFFER_SIZE);
	}

	
	const QString &command = request->command();
	QByteArray header = QString("%1 %2 %3").arg(command, QString::number(transaction_id), QString::number(dataLength)).toLocal8Bit();
	QByteArray newline = QByteArray(1, '\n');
	//qDebug() << "<<" << header << endl;
	this->socket_->write(header);
	this->socket_->write(newline);

	if (data != nullptr)
	{
		qint64 written = 0;
		while (written < dataLength)
		{
			
			qint64 remaining = dataLength - written;
			qint64 read = data->read(buffer, (remaining > UPLOAD_BUFFER_SIZE) ? UPLOAD_BUFFER_SIZE : remaining);
			qint64 this_write = this->socket_->write(buffer, read);
			written += this_write;
			if (this_write == 0)
			{
				if (this->socket_->state() != QTcpSocket::SocketState::ConnectedState)
				{
					disconnectFromEros();
					return;
				}
			}
			if (dataLength > UPLOAD_BUFFER_SIZE)
			{
				emit uploadProgress(written, dataLength);
			}
		}	

		free(buffer);
	}

	QTimer::singleShot(0, request, SLOT(requestSent()));
}

void Eros::requestComplete(Request* request) 
{
	QMap<int, Request*>::const_iterator iterator = this->requests_.constBegin();
	while (iterator != this->requests_.constEnd()) {
		if (iterator.value() == request)
		{
			this->requests_.remove(iterator.key());
			break;
		}
	}
}

void Eros::handshakeRequestComplete(Request* request) 
{
	if(HandshakeRequest* handshake_request = dynamic_cast<HandshakeRequest*>(request))
    {
		if (handshake_request->status() != HandshakeRequest::ResponseStatus::Success)
		{
			emit handshakeFailed();
			disconnectFromEros();
		}
		else
		{
			this->local_user_ = handshake_request->user();
			this->divisions_ = handshake_request->divisions();
			this->users_ <<  this->local_user_;

			QObject::connect(this->local_user_, SIGNAL(updated(User*)), this, SLOT(userUpdatedHandler(User*)));

			setState(ErosState::ConnectedState);
		}
	}
}

void Eros::matchmakingRequestComplete(Request *request)
{
	if(MatchmakingQueueRequest* matchmaking_request = dynamic_cast<MatchmakingQueueRequest*>(request))
    {
		if (matchmaking_request->status() == ErosMatchmakingState::Matched)
		{
			this->matchmaking_match_ = matchmaking_request->match();
		}

		setMatchmakingState(matchmaking_request->status());

	}
}

void Eros::matchmakingRequestQueued(Request *request)
{
	if(MatchmakingQueueRequest* matchmaking_request = dynamic_cast<MatchmakingQueueRequest*>(request))
    {
		setMatchmakingState(matchmaking_request->status());
	}
}

void Eros::requestError(Request* request, int code)
{
	emit serverError(code);
}


void Eros::userUpdatedHandler(User *user)
{
	if (user == this->local_user_)
	{
		emit localUserUpdated(this->local_user_);
	}
	else 
	{
		emit userUpdated(user);
	}
}

void Eros::characterUpdatedHandler(Character *character)
{
	emit characterUpdated(character);
}

int Eros::regionSearchingUserCount(ErosRegion region) const
{
	if (this->eros_matchmaking_searching_regions_.contains(region))
	{
		return this->eros_matchmaking_searching_regions_[region];
	}
	
	return 0;
}

// Update region stats, and emit the signal if there's a change.
void Eros::setRegionStats(ErosRegion region, int searching)
{
	if (this->eros_matchmaking_searching_regions_.contains(region))
	{
		if (this->eros_matchmaking_searching_regions_[region] == searching) 
		{
			return;
		}
	}

	this->eros_matchmaking_searching_regions_[region] = searching;
	emit regionStatsUpdated(region, searching);
}

// Update Eros stats, and emit the signal if there's a change.
void Eros::setStats(int active, int searching)
{
	if (active == this->eros_active_user_count_  && searching == this->eros_searching_user_count_)
	{
		return;
	}

	this->eros_active_user_count_ = active;
	this->eros_searching_user_count_ = searching;

	emit statsUpdated(active, searching);
}

int Eros::activeUserCount() const
{
	return this->eros_active_user_count_;
}

int Eros::searchingUserCount() const
{
	return this->eros_searching_user_count_;
}

void Eros::handleUserStats(const protobufs::UserStats &stats)
{
	if (stats.has_username())
	{
		QString username = QString::fromStdString(stats.username());
		if (this->local_user_->username() == username) 
		{
			this->local_user_->update(stats);
		}
		else 
		{
			QString username_key = username.toLower();
			User* user = nullptr;

			QList<User*>::iterator i;
			for (i = this->users_.begin(); i != this->users_.end(); ++i)
			{
				if ((*i)->username().toLower() == username_key)
				{
					user = (*i);
					break;
				}
			}

			if (user == nullptr)
			{
				user = new User(this, username);
				users_ << user;
				emit userAdded(user);
				user->update(stats);
			}
			else
			{
				user->update(stats);
			}
			
		}
	}
}

void Eros::sendMessage(ChatRoom *room, const QString message)
{
	ChatMessageRequest *request = new ChatMessageRequest(this, room, message);
	QObject::connect(request, SIGNAL(complete(Request*)), this, SLOT(chatMessageRequestComplete(Request*)));
	sendRequest(request);
}
void Eros::sendMessage(User *user, const QString message)
{
	PrivateMessageRequest *request = new PrivateMessageRequest(this, user, message);
	QObject::connect(request, SIGNAL(complete(Request*)), this, SLOT(chatMessageRequestComplete(Request*)));
	sendRequest(request);
}

void Eros::chatIndexRequestComplete(Request *request)
{
	if(ChatIndexRequest* index_request = dynamic_cast<ChatIndexRequest*>(request))
    {
		if (index_request->status() == ChatIndexRequest::RequestStatus::Success)
		{
			const QList<ChatRoom*> &rooms = index_request->chatRooms();
			QList<ChatRoom*>::const_iterator i;

			// Compare the index to our known rooms. Add any rooms we don't have.
			for (i = rooms.begin(); i != rooms.end(); i++)
			{
				if (this->chatrooms_.contains(*i))
				{
					continue;
				}

				this->chatrooms_ << (*i);
				emit chatRoomAdded(*i);
			}

			// Compare our known rooms to the index. Remove rooms that no longer exist.
			for (i = this->chatrooms_.begin(); i != this->chatrooms_.end(); i++)
			{
				if (rooms.contains(*i))
				{
					continue;
				}

				this->chatrooms_.removeAll(*i);
				emit chatRoomRemoved(*i);
			}

		}
	}
}

void Eros::chatJoinRequestComplete(Request *request)
{
	if(ChatJoinRequest* join_request = dynamic_cast<ChatJoinRequest*>(request))
    {
		if (join_request->status() != ChatJoinRequest::RequestStatus::Success)
		{
			emit chatRoomJoinFailed(join_request->room(), (ErosError)join_request->status());
		}
		else
		{
			if (!this->chatrooms_.contains(join_request->room()))
			{
				this->chatrooms_ << join_request->room();
			}
		}
	}
}

void Eros::chatLeaveRequestComplete(Request *request)
{
	if(ChatLeaveRequest* leave_request = dynamic_cast<ChatLeaveRequest*>(request))
    {
		emit chatRoomUserLeft(leave_request->room(), local_user_);
		emit chatRoomLeft(leave_request->room());
	}
}

void Eros::chatMessageRequestComplete(Request *request)
{
	if(PrivateMessageRequest* message_request = dynamic_cast<PrivateMessageRequest*>(request))
    {
		if (message_request->status() != ChatJoinRequest::RequestStatus::Success)
		{
			emit chatMessageFailed(message_request->user(), message_request->message(), (ErosError)message_request->status());
		}
		else 
		{
			emit chatMessageSent(message_request->user(), message_request->message());
		}
	}
	else if(ChatMessageRequest* message_request = dynamic_cast<ChatMessageRequest*>(request))
    {
		if (message_request->status() != ChatJoinRequest::RequestStatus::Success)
		{
			emit chatMessageFailed(message_request->room(), message_request->message(), (ErosError)message_request->status());
		}
		else 
		{
			emit chatMessageSent(message_request->room(), message_request->message());
		}
	}
}

const QList<ChatRoom *> &Eros::chatRooms() const
{
	return this->chatrooms_;
}

LocalUser *Eros::localUser()
{
	return this->local_user_;
}

void Eros::joinChatRoom(ChatRoom *room)
{
	this->joinChatRoom(room, "");
}

void Eros::joinChatRoom(ChatRoom *room, const QString password)
{
	ChatJoinRequest *request = new ChatJoinRequest(this, room, password);
	QObject::connect(request, SIGNAL(complete(Request*)), this, SLOT(chatJoinRequestComplete(Request*)));
	sendRequest(request);
}

void Eros::leaveChatRoom(ChatRoom *room)
{
	ChatLeaveRequest *request = new ChatLeaveRequest(this, room);
	QObject::connect(request, SIGNAL(complete(Request*)), this, SLOT(chatLeaveRequestComplete(Request*)));
	sendRequest(request);
}

void Eros::characterRequestComplete(Request *request)
{
	if(AddCharacterRequest* character_request = dynamic_cast<AddCharacterRequest*>(request))
    {
		if (character_request->status() == AddCharacterRequest::RequestStatus::Success) 
		{
			Character *character = character_request->character();
			emit characterAdded(character);
		} 
		else 
		{
			emit addCharacterError(character_request->battle_net_url(), (ErosError)character_request->status());
		}
	}
	else if(UpdateCharacterRequest* character_request = dynamic_cast<UpdateCharacterRequest*>(request))
    {
		// Don't need to emit character updated ourselves. It's done in the character->update handler.
		if (character_request->status() != AddCharacterRequest::RequestStatus::Success) 
		{
			Character *character = character_request->character();
			emit updateCharacterError(character, (ErosError)character_request->status());
		} 
	}
	else if(RemoveCharacterRequest* character_request = dynamic_cast<RemoveCharacterRequest*>(request))
    {
		Character *character = character_request->character();
		if (character_request->status() != AddCharacterRequest::RequestStatus::Success) 
		{
			emit removeCharacterError(character, (ErosError)character_request->status());
		} 
		else
		{
			emit characterRemoved(character);
		}
	}
}

void Eros::addCharacter(const QString battle_net_url)
{
	AddCharacterRequest *request = new AddCharacterRequest(this, battle_net_url);
	QObject::connect(request, SIGNAL(complete(Request*)), this, SLOT(characterRequestComplete(Request*)));
	sendRequest(request);
}

void Eros::updateCharacter(Character *character, int new_character_code, const QString new_game_profile_link)
{
	UpdateCharacterRequest *request = new UpdateCharacterRequest(this, character, new_character_code, new_game_profile_link);
	QObject::connect(request, SIGNAL(complete(Request*)), this, SLOT(characterRequestComplete(Request*)));
	sendRequest(request);
}

void Eros::removeCharacter(Character *character)
{
	RemoveCharacterRequest *request = new RemoveCharacterRequest(this, character);
	QObject::connect(request, SIGNAL(complete(Request*)), this, SLOT(characterRequestComplete(Request*)));
	sendRequest(request);
}


void Eros::replayRequestComplete(Request *request)
{
	if(UploadReplayRequest* replay_request = dynamic_cast<UploadReplayRequest*>(request))
    {
		if (replay_request->error() != ErosError::None)
		{
			emit replayUploadError(replay_request->error());
		}
		else
		{
			emit replayUploaded();
		}
	}

}

void Eros::uploadReplay(const QString path)
{
	QFile replay(path);
	if (!replay.open(QIODevice::ReadOnly)) {
		emit replayUploadError(ErosError::DiskReadError);
		return;
	}

	uploadReplay(&replay);
}

void Eros::uploadReplay(QIODevice *device)
{
	UploadReplayRequest	 *request = new UploadReplayRequest(this, device);
	QObject::connect(request, SIGNAL(complete(Request*)), this, SLOT(replayRequestComplete(Request*)));
	sendRequest(request);
}