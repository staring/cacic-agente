#-------------------------------------------------
#
# Project created by QtCreator 2014-08-13T13:28:50
#
#-------------------------------------------------

##############################################################################
#SEMPRE MUDAR A VERSÃO DE BUILD (o terceiro número) AO REALIZAR QUALQUER BUILD.#
VERSION_MAJOR = 3
VERSION_MINOR = 1
VERSION_BUILD = 9
DEFINES += "VERSION_MAJOR=$$VERSION_MAJOR"\
           "VERSION_MINOR=$$VERSION_MINOR"\
           "VERSION_BUILD=$$VERSION_BUILD"
VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}
##############################################################################

QT       += core
QT       += network
QT       -= gui

QMAKE_CXXFLAGS_WARN_ON = -Wall -Wno-unused-parameter -fpermissive

TARGET    = gercols

TEMPLATE = app

SOURCES += \
    main.cpp \
    ./gercols.cpp \
    ../src/ccacic.cpp \
    ../src/cacic_computer.cpp \
    ../src/cacic_software.cpp \
    ../src/cacic_hardware.cpp \
    ../src/operatingsystem.cpp \
    ../src/ccoleta.cpp \
    ../src/vregistry.cpp \
    ../src/vqtconvert.cpp \
    ../src/wmi.cpp \
    ../src/QLogger.cpp \
    ../src/identificadores.cpp \
    ../src/servicecontroller.cpp \
    ../src/logcacic.cpp

HEADERS += \
    ./gercols.h \
    ../src/ccacic.h \
    ../src/cacic_computer.h \
    ../src/cacic_software.h \
    ../src/cacic_hardware.h \
    ../src/operatingsystem.h \
    ../src/console.h \
    ../src/ccoleta.h \
    ../src/vregistry.h \
    ../src/vqtconvert.h \
    ../src/wmi.h \
    ../src/QLogger.h \
    ../src/identificadores.h \
    ../src/servicecontroller.h \
    ../src/logcacic.h

CONFIG   += console
CONFIG   -= app_bundle
#CONFIG   += qt
CONFIG   += c++11
CONFIG   += static

win32 {
    QT   += axcontainer
#    LIBS += -LC:\devel\cacic-agente\src\crypto++\lib -lcryptopp
} else {
#    LIBS += -L/usr/lib -lcryptopp
}



INCLUDEPATH += ../src/ \
#               ../src/crypto++/include/

DEFINES += STATIC
