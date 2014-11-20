#-------------------------------------------------
#
# Project created by QtCreator 2014-11-06T07:31:29
#
#-------------------------------------------------

QT       += core
QT       += dbus
QT       += network

QT       -= gui
DBUS_INTERFACES += mpreis.xml

TARGET = mqttToDbusBridge
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    mqttbridge.cpp \
    imagetobase64.cpp \
    mediaModalityController.cpp \
    tracklisttojsonstring.cpp \
    metadata.cpp

HEADERS += \
    libraries/qmqtt.h \
    libraries/qmqtt_client.h \
    libraries/qmqtt_message.h \
    libraries/qmqtt_will.h \
    mqttbridge.h \
    mqtt_strings_c.h \
    imagetobase64.h \
    mediaModalityController.h \
    tracklisttojsonstring.h \
    meta.h \
    metadata.h



unix:!macx: LIBS += -L$$PWD/libraries/ -lqmqtt

INCLUDEPATH += $$PWD/libraries
DEPENDPATH += $$PWD/libraries

OTHER_FILES += \
    mpreis.xml
