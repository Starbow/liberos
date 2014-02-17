QT       += network

QT       -= gui

TARGET = liberos
TEMPLATE = lib
CONFIG += staticlib

SOURCES += character.cpp\
           chatroom.cpp\
           divisions.cpp\
           eros.cpp\
           eros.pb.cc\
           localuser.cpp\
           matchmakingmatch.cpp\
           request.cpp\
           user.cpp\
           requests/chatindexrequest.cpp\
           requests/chatjoinrequest.cpp\
           requests/chatleaverequest.cpp\
           requests/chatmessagerequest.cpp\
           requests/handshakerequest.cpp\
           requests/matchmakingdequeuerequest.cpp\
		   requests/matchmakingforefeitrequest.cpp\
           requests/matchmakingqueuerequest.cpp\
           requests/pingrequest.cpp\
           requests/privatemessagerequest.cpp\
           requests/uploadreplayrequest.cpp\
           requests/addcharacterrequest.cpp\
           requests/removecharacterrequest.cpp\
           requests/updatecharacterrequest.cpp

HEADERS += character.h\
           chatroom.h\
           divisions.h\
           eros.h\
           eros.pb.h\
           localuser.h\
           matchmakingmatch.h\
           request.h\
           user.h\
           requests/chatindexrequest.h\
           requests/chatjoinrequest.h\
           requests/chatleaverequest.h\
           requests/chatmessagerequest.h\
           requests/handshakerequest.h\
           requests/matchmakingdequeuerequest.h\
		   requests/matchmakingforefeitrequest.h\
           requests/matchmakingqueuerequest.h\
           requests/pingrequest.h\
           requests/privatemessagerequest.h\
           requests/uploadreplayrequest.h\
           requests/addcharacterrequest.h\
           requests/removecharacterrequest.h\
           requests/updatecharacterrequest.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
