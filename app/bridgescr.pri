QT       += core
QT       += dbus
QT       += network

QT       -= gui

DBUS_INTERFACES += mpreis.xml

SOURCES += mqttbridge.cpp \
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
