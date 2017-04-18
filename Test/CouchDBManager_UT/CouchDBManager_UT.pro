#-------------------------------------------------
#
# Project created by QtCreator 2016-07-12T09:27:06
#
#-------------------------------------------------

QT       += network testlib

QT       -= gui

TARGET = tst_couchdbmanager_unittest
CONFIG   += console
CONFIG   += c++11
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_couchdbmanager_unittest.cpp \
    device.cpp \
    physical_car.cpp \
    physical_device.cpp \
    physical_ut.cpp \
    physical_ut_car.cpp \
    variables.cpp \
    bus.cpp \
    auxiliary.cpp \
    types.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../Bin/release/ -lCouchDBManager
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../Bin/debug/ -lCouchDBManager
else:unix: LIBS += -L$$PWD/../../Bin/ -lCouchDBManager

INCLUDEPATH += $$PWD/../../Include
DEPENDPATH += $$PWD/../../Include

HEADERS += \
    car.h \
    device.h \
    physical_car.h \
    physical_device.h \
    physical_ut.h \
    physical_ut_car.h \
    ut.h \
    variables.h \
    bus.h \
    rs485cosmos.h \
    monitconfig.h \
    auxiliary.h \
    types.h
