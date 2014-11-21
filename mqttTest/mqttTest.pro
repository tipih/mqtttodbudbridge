#-------------------------------------------------
#
# Project created by QtCreator 2014-11-21T08:44:41
#
#-------------------------------------------------

QT       += network testlib
QT       -= gui
QT       += core
QT       += dbus

DBUS_INTERFACES += $$PWD/../app/mpreis.xml

TARGET = tst_mqtttesttest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app



SOURCES += tst_mqtttesttest.cpp \
            ../app/metadata.cpp \
            ../app/tracklisttojsonstring.cpp

HEADERS += ../app/tracklisttojsonstring.h

DEFINES += SRCDIR=\\\"$$PWD/\\\"
INCLUDEPATH += $$PWD/../app \
               $$PWD/../app/libraries

OTHER_FILES += \
    $$PWD/../app/mpreis.xml

unix:!macx: LIBS += -L$$PWD/../app/libraries/ -lqmqtt
DEPENDPATH += $$PWD/../app/libraries
