CONFIG += testcase

TARGET = tst_touchmouse
QT += core-private gui-private qml-private quick-private v8-private testlib

macx:CONFIG -= app_bundle

SOURCES  += tst_touchmouse.cpp

include (../../shared/util.pri)

TESTDATA = data/*

# OTHER_FILES += data/foo.qml

CONFIG += parallel_test

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0
