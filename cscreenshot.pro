#-------------------------------------------------
#
# Project created by QtCreator 2015-09-06T15:24:46
#
#-------------------------------------------------

QT       += core gui
QT += macextras
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cscreenshot
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    cscreenshotview.cpp \
    cscreenshotscene.cpp \
    cscreenshotmanager.cpp \
    cscreenshot.cpp \
    cscreenrectitem.cpp \
    csreeneditorwidget.cpp

HEADERS  += dialog.h \
    cscreenshotview.h \
    cscreenshotscene.h \
    cscreenshotmanager.h \
    cscreenshot.h \
    cscreenrectitem.h \
    cscreenpublic.h \
    csreeneditorwidget.h

FORMS    += dialog.ui \
    csreeneditorwidget.ui
