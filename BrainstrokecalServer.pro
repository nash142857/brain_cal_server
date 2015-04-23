#-------------------------------------------------
#
# Project created by QtCreator 2015-04-07T21:43:23
#
#-------------------------------------------------

QT       += core network gui sql script

include(qt-components/QsLog/QsLog.pri)
include(QtWebApp/httpserver/httpserver.pri)
TARGET = BrainstrokecalServer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

OTHER_FILES += etc/BrainstrokecalServer.ini
OTHER_FILES += data/*
OTHER_FILES += conf/*
OTHER_FILES += log/*


LIBS += -lz /Users/gsl/dcmtk/lib/libcharls.a \
    /Users/gsl/dcmtk/lib/libdcmdata.a \
    /Users/gsl/dcmtk/lib/libdcmdsig.a \
    /Users/gsl/dcmtk/lib/libdcmimage.a \
    /Users/gsl/dcmtk/lib/libdcmimgle.a \
    /Users/gsl/dcmtk/lib/libdcmjpeg.a \
    /Users/gsl/dcmtk/lib/libdcmjpls.a \
    /Users/gsl/dcmtk/lib/libdcmnet.a \
    /Users/gsl/dcmtk/lib/libdcmpstat.a \
    /Users/gsl/dcmtk/lib/libdcmqrdb.a \
    /Users/gsl/dcmtk/lib/libdcmsr.a \
    /Users/gsl/dcmtk/lib/libdcmtls.a \
    /Users/gsl/dcmtk/lib/libdcmwlm.a \
    /Users/gsl/dcmtk/lib/libijg8.a \
    /Users/gsl/dcmtk/lib/libijg12.a \
    /Users/gsl/dcmtk/lib/libijg16.a \
    /Users/gsl/dcmtk/lib/liblibi2d.a \
    /Users/gsl/dcmtk/lib/liboflog.a \
    /Users/gsl/dcmtk/lib/libofstd.a \
    /usr/local/lib/libfftw3f.a \
    /usr/local/lib/libfftw3.a \



INCLUDEPATH += /Users/gsl/dcmtk/include/
INCLUDEPATH += /usr/local/include/

SOURCES += main.cpp \
    UsefullTools.cpp \
    getDcm.cpp \
    contable.cpp \
    DCMCompute.cpp \
    DicomInfo.cpp \
    BMP.cpp \
    SortDcmFile.cpp \
    calculate_method.cpp \
    brain_stroke_server.cpp \
    tcp_deal.cpp \
    calculate_thread.cpp \
    brain_stroke_web.cpp \
    get_version_controller.cpp \
    rename_version_controller.cpp \
    file_control.cpp \
    download_version_controller.cpp

HEADERS += \
    UsefullTools.h \
    symbol.h \
    simmath.h \
    PWIInfo.h \
    getDcm.h \
    DicomInfo.h \
    contable.h \
    DCMHeaders.h \
    common.h \
    QTSource.h \
    DCMCompute.h \
    BMP.h \
    SortDcmFile.h \
    calculate_method.h \
    config.h \
    brain_stroke_server.h \
    tcp_deal.h \
    calculate_thread.h \
    brain_stroke_web.h \
    get_version_controller.h \
    rename_version_controller.h \
    file_control.h \
    download_version_controller.h

