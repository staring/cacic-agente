#-------------------------------------------------
#
# Project created by QtCreator 2014-08-13T13:28:50
#
#-------------------------------------------------

QT       += core
QT       += network
QT       -= gui

TARGET = gercols
CONFIG   += console
CONFIG   -= app_bundle

win32 {
    LIBS += -LE:\LightBase\cacic-agente-project\cacic-agente\src\crypto++\lib -lcryptopp
} else {
    LIBS += -L/usr/lib -lcryptopp
}

TEMPLATE = app

SOURCES += \
    main.cpp \
    ../src/ccacic.cpp \
    ../src/cacic_computer.cpp


HEADERS += \
    ../src/ccacic.h \
    ../src/cacic_computer.h \
    ../src/cacic_comm.h

INCLUDEPATH += ../src \
               ../src/crypto++/include/