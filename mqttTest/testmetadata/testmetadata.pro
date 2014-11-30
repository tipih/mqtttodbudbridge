#-------------------------------------------------
#
# Project created by QtCreator 2014-11-24T08:24:22
#
#-------------------------------------------------
include(../../defaults.pri)
QT += core
QT += dbus
QT += network
QT += gui
QT += testlib

TARGET = tst_testmetadatatest
CONFIG   += console
CONFIG   -= app_bundle
message($$PWD)
QMAKE_POST_LINK  = $${QMAKE_COPY} $$PWD/Musicplaylist.xspf  $$OUT_PWD

TEMPLATE = app
LIBS += -L../../src -lmyapp

SOURCES += tst_testmetadatatest.cpp

