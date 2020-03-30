#-------------------------------------------------
#
# Project created by QtCreator 2020-03-30T16:22:54
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = testTelNet
TEMPLATE = app

win32 {
    contains(TEMPLATE, lib):contains(CONFIG, shared):DEFINES += QT_QTTELNET_EXPORT
    else:qttelnet-uselib:DEFINES += QT_QTTELNET_IMPORT
}

win32:LIBS += -lWs2_32

SOURCES += main.cpp\
        mainwindow.cpp \
    qttelnet.cpp

HEADERS  += mainwindow.h \
    qttelnet.h

FORMS    += mainwindow.ui
