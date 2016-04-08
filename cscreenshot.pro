#-------------------------------------------------
#
# Project created by QtCreator 2016-03-31T16:49:29
#
#-------------------------------------------------
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

mac{
QT += macextras
DESTDIR = ../cscreenshot/lib/mac
QMAKE_LFLAGS += -framework AppKit
OBJECTIVE_SOURCES += \
    ./src/cscreenpublic.mm
}
win32 {
SOURCES += ./src/cscreenpublic.cpp
DESTDIR = ../cscreenshot/lib/win
#DLLDESTDIR +=../cscreenshot/lib/win
        LIBS += -luser32
}

TARGET = cscreenshot
TEMPLATE = lib

DEFINES += CSCREENSHOT_LIBRARY
INCLUDEPATH += ./include

SOURCES += ./src/cscreenshotmanager.cpp \
    ./src/cscreenattributetoolbaritem.cpp \
    ./src/cscreencolortoolbaritem.cpp \
    ./src/cscreeneditorbuttonitem.cpp \
    ./src/cscreeneditortoolbaritem.cpp \
    ./src/cscreenrectitem.cpp \
    ./src/cscreenselectrectitem.cpp \
    ./src/cscreenshotscene.cpp \
    ./src/cscreenshotview.cpp \
    ./src/cscreentooltipitem.cpp

HEADERS += ./include/cscreenshotmanager.h\
        ./include/cscreenshot_global.h \
    ./include/cscreenattributetoolbaritem.h \
    ./include/cscreencolortoolbaritem.h \
    ./include/cscreeneditorbuttonitem.h \
    ./include/cscreeneditortoolbaritem.h \
    ./include/cscreenpublic.h \
    ./include/cscreenrectitem.h \
    ./include/cscreenselectrectitem.h \
    ./include/cscreenshotscene.h \
    ./include/cscreenshotview.h \
    ./include/cscreentooltipitem.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    source.qrc

DISTFILES +=

