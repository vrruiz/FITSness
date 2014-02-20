#-------------------------------------------------
#
# Project created by QtCreator 2013-07-31T00:43:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FITSness
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    histogram.cpp \
    fitsimage.cpp \
    dialogshowheaders.cpp

HEADERS  += mainwindow.h \
    histogram.h \
    fitsimage.h \
    dialogshowheaders.h

win32: HEADERS += cfitsio/longnam.h cfitsio/fitsio.h
win32: LIBS += -L$$PWD/cfitsio/ -lcfitsio
win32: INCLUDEPATH += $$PWD/cfitsio
win32: DEPENDPATH += $$PWD/cfitsio

macx: INCLUDEPATH += /opt/local/include/
macx: LIBS += -L/opt/local/lib/ -lcfitsio

FORMS    += mainwindow.ui \
    dialogshowheaders.ui
RESOURCES = images.qrc
