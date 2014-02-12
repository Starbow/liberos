QT       += network

QT       -= gui

TARGET = liberos
TEMPLATE = lib
CONFIG += staticlib

SOURCES += character.cpp\
           chatroom.cpp\
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
           requests/matchmakingqueuerequest.cpp\
           requests/pingrequest.cpp\
           requests/privatemessagerequest.cpp

HEADERS += character.h\
           chatroom.h\
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
           requests/matchmakingqueuerequest.h\
           requests/pingrequest.h\
           requests/privatemessagerequest.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
