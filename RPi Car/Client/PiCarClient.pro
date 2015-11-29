#-------------------------------------------------
#
# Project created by QtCreator 2015-09-21T22:06:58
#
#-------------------------------------------------

QT       += core gui multimedia multimedia-private network multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PiCarClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
LIBS += -lws2_32
CONFIG += c++11
