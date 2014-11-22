include(../defaults.pri)



QT +=testlib


TARGET = tst_mqtttesttest
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += tst_mqtttesttest.cpp \



LIBS += -L../src -lmyapp

RESOURCES += \
    image.qrc
