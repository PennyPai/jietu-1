#-------------------------------------------------
#
# Project created by QtCreator 2016-03-31T18:05:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cscreenshottest
TEMPLATE = app

INCLUDEPATH += ../../include
LIBS += -L../../lib/mac -lcscreenshot

SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h

FORMS    += widget.ui
