CONFIG += testcase
TEMPLATE = app
TARGET = tst_bench_qjsvalue
DEPENDPATH += .
INCLUDEPATH += .
macx:CONFIG -= app_bundle
CONFIG += release

SOURCES += tst_qjsvalue.cpp

QT += core-private v8-private qml-private testlib
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0
