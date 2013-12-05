#-------------------------------------------------
#
# Project created by QtCreator 2013-12-03T14:40:34
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cc
TEMPLATE = app


SOURCES += main.cpp\
        cc.cpp \
    login.cpp

HEADERS  += cc.h \
    login.h

FORMS    += cc.ui \
    login.ui

RESOURCES += \
    cc.qrc
