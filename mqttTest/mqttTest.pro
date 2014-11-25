TEMPLATE = subdirs
CONFIG+=ordered
SUBDIRS = \
    testimagetobase64 \
    testmetadata \
    testtracklistclass
   

app.depends = src
mqttTest.depends = src

OTHER_FILES += \
    defaults.pri
