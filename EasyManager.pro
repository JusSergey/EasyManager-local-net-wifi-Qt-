#-------------------------------------------------
#
# Project created by QtCreator 2016-04-05T20:25:12
#
#-------------------------------------------------

# Fucking Files, and PC Manager.

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -std=c++11

#LIBS += -lX11

TARGET = EasyManager
TEMPLATE = app


SOURCES += main.cpp \
    NetworkManager.cpp \
    function.cpp \
    filemanager.cpp \
    server.cpp \
    terminal.cpp \
    pcmanager.cpp \
    info.cpp \
    mytoolbutton.cpp \
    mylistwidget.cpp \
    sysmouse.cpp \
    screendisplay.cpp

HEADERS  += \
    NetwotkManager.h \
    function.h \
    constant.h \
    filemanager.h \
    server.h \
    terminal.h \
    pcmanager.h \
    info.h \
    mytoolbutton.h \
    mylistwidget.h \
    sysmouse.h \
    screendisplay.h

#CONFIG += mobility
#MOBILITY =

RESOURCES += \
    resource.qrc

