#-------------------------------------------------
#
# Project created by QtCreator 2017-04-19T08:09:00
#
#-------------------------------------------------

QT       += network testlib

QT       -= gui

TARGET = tst_couchdbmanagertest
CONFIG   += console
CONFIG   += c++11
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_couchdbmanagertest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../Bin/release/ -lCouchDBManager
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../Bin/debug/ -lCouchDBManager
else:unix: LIBS += -L$$PWD/../../Bin/ -lCouchDBManager

INCLUDEPATH += $$PWD/../../Include
DEPENDPATH += $$PWD/../../Include

HEADERS += \
    car.h
