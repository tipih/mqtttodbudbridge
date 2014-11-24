#-------------------------------------------------
#
# Project created by QtCreator 2014-11-24T08:24:22
#
#-------------------------------------------------
include(../../defaults.pri)
QT       += testlib dbus

QT       -= gui

TARGET = tst_testmetadatatest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
LIBS += -L../../src -lmyapp

SOURCES += tst_testmetadatatest.cpp

