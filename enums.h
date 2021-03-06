#ifndef LIBEROS_ENUMS_H
#define LIBEROS_ENUMS_H

#include <QList>

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

typedef QList<ErosRegion> ErosRegionList;
Q_DECLARE_METATYPE(ErosRegionList)


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
	CannotPerformActionWhileMatchmaking = 109,

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
	BadSpeed = 311,
	CannotVetoUnrankedMap = 312,
	MaxVetoesReached = 313, // Put your motherf**kin' hands up and follow me
	GameNotPrearranged = 314,

	NoCharacterForRegion = 401,
	MatchmakingAborted = 402,
	LongProcessRequestFailed = 403,

	RoomNotJoinable = 501,
	BadPassword = 502,
	RoomAlreadyExists = 503,
	RoomReserved = 504,
	MaximumRoomLimitReached = 505,
	NotOnChannel = 506,
	UserNotFound = 507,
	BadMessage = 508,
	NameTooShort = 509,
	RateLimited = 511,
};


Q_DECLARE_METATYPE(ErosError)

enum ErosLongProcessState
{
	LongProcessIdle,
	FlaggedNoShow,
	OpponentFlaggedNoShow,
	DrawRequest,
	OpponentFlaggedDrawRequest,
};

Q_DECLARE_METATYPE(ErosLongProcessState)

#endif