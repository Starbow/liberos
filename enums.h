#ifndef LIBEROS_ENUMS_H
#define LIBEROS_ENUMS_H

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

enum ErosError
{
	None = 0,

	DatabaseReadError = 101,
	DatabaseWriteError = 102,
	DiskReadError = 103,
	DiskWriteError = 104,
	AuthenticationError = 105,
	GenericError = 106,
	BadName = 107,
	NameInUse = 108,

	BadCharacterInfo = 201,
	CharacterExists = 202,
	BattleNetCommunicationError = 203,
	VerificationFailed = 204,

	ReplayProcessingError = 301,
	MatchProcessingError = 302,
	DuplicateReplay = 303,
	ClientNotInvolvedInMatch = 304,
	GameTooShort = 305,
	BadFormat = 306,
	BadMap = 307,
	InvalidParticipants = 308,
	PlayerNotInDatabase = 309,
	NotAssignedOpponent = 310,

	NoCharacterForRegion = 401,
	MatchmakingAborted = 402,

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


Q_DECLARE_METATYPE(ErosError)
#endif