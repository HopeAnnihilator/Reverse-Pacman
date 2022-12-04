#-------------------------------------------------
#
# Project created by QtCreator 2022-11-07T12:30:48
#
#-------------------------------------------------

QT       += core gui
QT       += websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ReversePacMan
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        src/game.cpp \
        src/objects/gameObject.cpp \
        src/objects/player.cpp \
        src/objects/projectile.cpp \
        src/objects/sprite.cpp \
        src/scenes/gameInterface.cpp \
        src/scenes/gameover.cpp \
        src/scenes/leaderboard.cpp \
        src/scenes/titlescreen.cpp \
        src/sockets/wsClient.cpp

HEADERS += \
        src/game.h \
        src/objects/gameObject.h \
        src/objects/player.h \
        src/defs.h \
        src/objects/projectile.h \
        src/objects/sprite.h \
        src/scenes/gameInterface.h \
        src/scenes/gameover.h \
        src/scenes/leaderboard.h \
        src/scenes/titlescreen.h \
        src/sockets/wsClient.h

FORMS += \
        src/scenes/gameInterface.ui \
        src/scenes/gameover.ui \
        src/scenes/leaderboard.ui \
        src/scenes/titlescreen.ui

RESOURCES += \
    src/images/spriteMap.png \
    src/images/spriteMapScaled48x48.png \
    src/objects/map.txt


