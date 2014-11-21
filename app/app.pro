#-------------------------------------------------
#
# Project created by QtCreator 2014-11-06T07:31:29
#
#-------------------------------------------------




TARGET = mqttToDbusBridge
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


include (bridgescr.pri)
SOURCES += main.cpp


unix:!macx: LIBS += -L$$PWD/libraries/ -lqmqtt

INCLUDEPATH += $$PWD/libraries
DEPENDPATH += $$PWD/libraries

OTHER_FILES += \
    mpreis.xml
