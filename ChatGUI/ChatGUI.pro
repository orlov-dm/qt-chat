#-------------------------------------------------
#
# Project created by QtCreator 2017-07-17T20:05:18
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChatGUI
TEMPLATE = app


SOURCES += main.cpp\
        ui/mainwindow.cpp \
    ui/logindialog.cpp \
    core/usersmodel.cpp \
    ../ChatCommon/common.cpp

HEADERS  += ui/mainwindow.h \
    ui/logindialog.h \
    core/usersmodel.h

FORMS    += ui/mainwindow.ui \
    ui/logindialog.ui

INCLUDEPATH += "../ChatCommon/"
