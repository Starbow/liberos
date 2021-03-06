QT       += network

QT       -= gui

TARGET = eros
TEMPLATE = lib
CONFIG += staticlib c++11

SOURCES += character.cpp\
           chatroom.cpp\
           division.cpp\
           eros.cpp\
           eros.pb.cc\
           localuser.cpp\
           map.cpp\
           matchmakingmatch.cpp\
           request.cpp\
           user.cpp\
           userladderstats.cpp\
           requests/chatindexrequest.cpp\
           requests/chatjoinrequest.cpp\
           requests/chatleaverequest.cpp\
           requests/chatmessagerequest.cpp\
           requests/handshakerequest.cpp\
           requests/matchmakingdequeuerequest.cpp\
           requests/matchmakingforfeitrequest.cpp\
           requests/matchmakingqueuerequest.cpp\
           requests/pingrequest.cpp\
           requests/privatemessagerequest.cpp\
           requests/uploadreplayrequest.cpp\
           requests/addcharacterrequest.cpp\
           requests/removecharacterrequest.cpp\
           requests/updatecharacterrequest.cpp\
           requests/longprocessrequest.cpp\
           requests/longprocessresponserequest.cpp\
           requests/togglevetorequest.cpp

HEADERS += character.h\
           chatroom.h\
           division.h\
           enums.h\
           eros.h\
           eros.pb.h\
           localuser.h\
           map.h\
           matchmakingmatch.h\
           request.h\
           user.h\
           userladderstats.h\
           requests/chatindexrequest.h\
           requests/chatjoinrequest.h\
           requests/chatleaverequest.h\
           requests/chatmessagerequest.h\
           requests/handshakerequest.h\
           requests/matchmakingdequeuerequest.h\
           requests/matchmakingforfeitrequest.h\
           requests/matchmakingqueuerequest.h\
           requests/pingrequest.h\
           requests/privatemessagerequest.h\
           requests/uploadreplayrequest.h\
           requests/addcharacterrequest.h\
           requests/removecharacterrequest.h\
           requests/updatecharacterrequest.h\
           requests/longprocessrequest.h\
           requests/longprocessresponserequest.h\
           requests/togglevetorequest.h

unix {
    LIBS += -L/usr/local/lib/
    INCLUDEPATH += /usr/local/include/
    target.path = /usr/local/lib
    INSTALLS += target
}

TRANSLATIONS += translations/liberos_en_GB.ts\
                translations/liberos_en_US.ts\
                translations/liberos_de_DE.ts\
                translations/liberos_ko_KR.ts\
                translations/liberos_es_419.ts\
				translations/liberos_es_ES.ts\
				translations/liberos_fr_FR.ts\
                translations/liberos_blank.ts
