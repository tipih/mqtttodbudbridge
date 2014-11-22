include(../defaults.pri)
QT += core
QT += dbus
QT += network



CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp


LIBS += -L../src -lmyapp


