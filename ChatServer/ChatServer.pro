#-------------------------------------------------
#
# Project created by QtCreator 2017-07-16T20:48:41
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChatServer
TEMPLATE = app


SOURCES += main.cpp\
        ui/mainwindow.cpp \
    core/server.cpp \
    core/connection.cpp \
    ../ChatCommon/common.cpp

HEADERS  += ui/mainwindow.h \
    core/server.h \
    core/connection.h

FORMS    += ui/mainwindow.ui

INCLUDEPATH += "../ChatCommon/"
