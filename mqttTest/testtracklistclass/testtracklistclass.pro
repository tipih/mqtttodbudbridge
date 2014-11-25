#-------------------------------------------------
#
# Project created by QtCreator 2014-11-25T14:16:37
#
#-------------------------------------------------
include(../../defaults.pri)
QT       += testlib

QT       -= gui

TARGET = tst_testtracklistclasstest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

LIBS += -L../../src -lmyapp
SOURCES += tst_testtracklistclasstest.cpp

