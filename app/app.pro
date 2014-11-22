include(../defaults.pri)
QT += core
QT += dbus
QT += network
DBUS_INTERFACES += ../src/mpreis.xml


CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp


LIBS += -L../src -lmyapp

OTHER_FILES += \
   ../src/mpreis.xml
