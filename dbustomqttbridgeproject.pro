TEMPLATE = subdirs
CONFIG+=ordered
SUBDIRS = \
    src \
    app \
    mqttTest

app.depends = src
mqttTest.depends = src

OTHER_FILES += \
    defaults.pri
