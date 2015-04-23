QT += core

TARGET = QsTools
CONFIG += console qtestlib
CONFIG -= app_bundle
TEMPLATE = app

INCLUDEPATH += "../../QsTools" \
   "../"

# test-case sources
SOURCES += \
    TestVersion.cpp \
    TestLog.cpp

# component sources
include(../../QsLog/QsLog.pri)

SOURCES += \
    ../QtTestUtil/TestRegistry.cpp \
    ../QtTestUtil/SimpleChecker.cpp \
    ../../QsTools/QsVersion.cpp

HEADERS += \
    ../QtTestUtil/TestRegistry.h \
    ../QtTestUtil/TestRegistration.h \
    ../QtTestUtil/QtTestUtil.h \
    ../../QsTools/QsVersion.h
