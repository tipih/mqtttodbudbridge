include(../defaults.pri)

QT += core
QT += dbus
QT += network
QT += gui

TARGET = myapp
TEMPLATE = lib




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



unix:!macx: LIBS += -L$$PWD/libraries/ -lqmqtt

INCLUDEPATH += $$PWD/libraries
DEPENDPATH += $$PWD/libraries

OTHER_FILES += \
    mpreis.xml

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }

    INSTALLS += target

}
