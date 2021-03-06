#-----------------------------------------------------------------------------------------
#
# Project Cacic Agente, module cacic-service, created by Lightbase (http://www.softwarepublico.gov.br/ver-comunidade?community_id=3585)
#
# Developers: Eric Menezes Noronha (eric.m.noronha@lightbase.com.br); GitHub: ericmenezes
#             Thiago Rocha         (thiago.rocha@lightbase.com.br)  ;
#
#-----------------------------------------------------------------------------------------

##############################################################################
#SEMPRE MUDAR A VERSÃO DE BUILD (o terceiro número) AO REALIZAR QUALQUER BUILD.#
VERSION_MAJOR = 3
VERSION_MINOR = 2
VERSION_BUILD = 0
DEFINES += VERSION_MAJOR=$$VERSION_MAJOR \
           VERSION_MINOR=$$VERSION_MINOR \
           VERSION_BUILD=$$VERSION_BUILD

VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}
##############################################################################
@CONFIG += debug_and_release@
QT       += core
QT       -= gui
QT       -= widgets
QT       += network

QMAKE_CXXFLAGS_WARN_ON = -Wall -Wno-unused-parameter -fpermissive

TARGET       = cacicdeploy

CONFIG      += console
CONFIG      -= app_bundle
CONFIG      += static

TEMPLATE     = app
INCLUDEPATH += ../src \
#               ../../src/crypto++/include/

win32 {
#    LIBS    += -LC:\devel\cacic-agente\src\crypto++\lib -lcryptopp
    QT      += axcontainer
    LIBS += -lws2_32
    LIBS += -lshlwapi
    LIBS += -liphlpapi
} else {
#    LIBS    += -L/usr/lib -lcryptopp
}

SOURCES     += main.cpp \
               ../src/ccacic.cpp \
               ../src/QLogger.cpp \
               ../src/identificadores.cpp \
               ../src/servicecontroller.cpp \
               ../src/logcacic.cpp \
               ../src/cacic_computer.cpp \
               ../src/cacic_comm.cpp \
               ../src/operatingsystem.cpp \
               ../src/cacicthread.cpp \
               cacicdeploy.cpp \
               deploytimer.cpp \
               ../src/vregistry.cpp \
               ../src/vqtconvert.cpp \
               ../src/wmi.cpp \
               ../src/wcomputer.cpp


HEADERS     += cacicd.h \
               ../src/ccacic.h \
               ../src/QLogger.h \
               ../src/identificadores.h \
               ../src/servicecontroller.h \
               ../src/logcacic.h \
               ../src/cacic_computer.h \
               ../src/cacic_comm.h \
               ../src/operatingsystem.h \
               ../src/cacicthread.h \
               cacicdeploy.h \
               deploytimer.h \
               ../src/vregistry.h \
               ../src/vqtconvert.h \
               ../src/wmi.h \
               ../src/wcomputer.h

include(../src/qtservice/src/qtservice.pri)

