#-------------------------------------------------
#
# Project created by QtCreator 2016-05-11T13:45:46
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = CouchDBManager
TEMPLATE = lib

SCMAN_CODE="AD.37.91.3047"

DEFINES += COUCHDBMANAGER_LIBRARY
DEFINES += QT_NO_DEBUG_OUTPUT
DEFINES += QT_NO_DEBUG

ROOT_DIR=../../..
PROJECT_DIR=$$PWD/..

#include($$ROOT_DIR/Scm/AD.37.91.3001/build_rules.pri)


INCLUDEPATH += \
        $$PROJECT_DIR/Include \
        $$ROOT_DIR \
        $$ROOT_DIR/Include
		
SOURCES += \
    $$PROJECT_DIR/Src/baseentity.cpp \
    $$PROJECT_DIR/Src/baseobject.cpp \
    $$PROJECT_DIR/Src/basereference.cpp \
    $$PROJECT_DIR/Src/basereferencesimple.cpp \
    $$PROJECT_DIR/Src/couchdbmanager.cpp \
    $$PROJECT_DIR/Src/dbmanagerresponse.cpp \
    $$PROJECT_DIR/Src/relationentity.cpp \
    $$PROJECT_DIR/Src/replicationconfig.cpp \
    $$PROJECT_DIR/Src/usercontext.cpp \
    $$PROJECT_DIR/Src/versionableentity.cpp

HEADERS += \
    $$PROJECT_DIR/Include/activetask.h \
    $$PROJECT_DIR/Include/baseentity.h \
    $$PROJECT_DIR/Include/baseobject.h \
    $$PROJECT_DIR/Include/basereference.h \
    $$PROJECT_DIR/Include/basereferencesimple.h \
    $$PROJECT_DIR/Include/baseuser.h \
    $$PROJECT_DIR/Include/couchdbmanager.h \
    $$PROJECT_DIR/Include/couchdbmanager_debug.h \
    $$PROJECT_DIR/Include/couchdbmanager_global.h \
    $$PROJECT_DIR/Include/dbmanagerresponse.h \
    $$PROJECT_DIR/Include/relationentity.h \
    $$PROJECT_DIR/Include/replicationconfig.h \
    $$PROJECT_DIR/Include/usercontext.h \
    $$PROJECT_DIR/Include/versionableentity.h \

unix {
    target.path = /usr/lib
    INSTALLS += target
}


