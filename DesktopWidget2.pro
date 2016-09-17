#-------------------------------------------------
#
# Project created by QtCreator 2013-03-16T12:55:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DesktopWidget2
TEMPLATE = app


SOURCES += main.cpp\
        desktopwidget.cpp \
    desktopitem.cpp \
    utils.cpp \
    windowsettings.cpp \
    desktopwidgetdialog.cpp

HEADERS  += desktopwidget.h \
    desktopitem.h \
    utils.h \
    windowsettings.h \
    desktopwidgetdialog.h

FORMS    += desktopwidget.ui \
    desktopwidgetdialog.ui

RESOURCES += \
    resource.qrc
