#-------------------------------------------------
#
# Project created by QtCreator 2014-03-09T01:27:30
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProjMapper
TEMPLATE = app


SOURCES += main.cpp\
        mapwindow.cpp \
    outwindow.cpp \
    quad.cpp \
    outwidget.cpp \
    mapwidget.cpp \
    renderclass.cpp \
    quadconfigdialog.cpp

HEADERS  += mapwindow.h \
    outwindow.h \
    quad.h \
    outwidget.h \
    mapwidget.h \
    renderclass.h \
    quadconfigdialog.h

FORMS    += mapwindow.ui \
    outwindow.ui \
    quadconfigdialog.ui

RESOURCES += \
    TestTextures.qrc
