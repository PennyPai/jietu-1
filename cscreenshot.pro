#-------------------------------------------------
#
# Project created by QtCreator 2015-09-06T15:24:46
#
#-------------------------------------------------

QT       += core gui
mac{
QT += macextras
QMAKE_LFLAGS += -framework AppKit
OBJECTIVE_SOURCES += \
    cscreenpublic.mm
}

win32 {
#SOURCES += cscreenpublic.cpp
}
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cscreenshot
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    cscreenshotview.cpp \
    cscreenshotscene.cpp \
    cscreenshotmanager.cpp \
    cscreeneditortoolbaritem.cpp \
    cscreeneditorbuttonitem.cpp \
    cscreenselectrectitem.cpp \
    cscreenrectitem.cpp \
    cscreentooltipitem.cpp \
    cscreenattributetoolbaritem.cpp \
    cscreencolortoolbaritem.cpp

HEADERS  += dialog.h \
    cscreenshotview.h \
    cscreenshotscene.h \
    cscreenshotmanager.h \
    cscreenpublic.h \
    cscreeneditortoolbaritem.h \
    cscreeneditorbuttonitem.h \
    clogsetting.h \
    cscreenselectrectitem.h \
    cscreenrectitem.h \
    cscreentooltipitem.h \
    cscreenattributetoolbaritem.h \
    cscreencolortoolbaritem.h

FORMS    += dialog.ui
RESOURCES += \
    source.qrc

