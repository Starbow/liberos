#ifndef ENUMS_H
#define ENUMS_H

enum ErosState
{
	UnconnectedState,
	ConnectingState,
	HandshakingState,
	ConnectedState,
	DisconnectingState,
};

Q_DECLARE_METATYPE(ErosState)

enum ErosRegion
{
	NA = 1,
	EU = 2,
	KR = 3,
	CN = 5,
	SEA = 6,
};

Q_DECLARE_METATYPE(ErosRegion)

enum ErosMatchmakingState
{
	Idle = 0,
	Queued = 1,
	Matched = 2,
	InvalidRegion = 401,
	Aborted = 402,
};

Q_DECLARE_METATYPE(ErosMatchmakingState)

enum ErosUserState
{
	Unknown = 0,
	Known = 1,
};

Q_DECLARE_METATYPE(ErosUserState)

enum ErosChatError
{
	RoomNotJoinable = 501,
	BadPassword = 502,
	RoomAlreadyExists = 503,
	RoomReserved = 504,
	MaximumRoomLimitReached = 505,
	NotOnChannel = 506,
	UserNotFound = 507,
	BadMessage = 508,
	NameTooShort = 509,
};


Q_DECLARE_METATYPE(ErosChatError)
#endif