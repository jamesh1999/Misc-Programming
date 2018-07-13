#-------------------------------------------------
#
# Project created by QtCreator 2016-06-11T14:20:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NeuralTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    creature.cpp \
    neuron.cpp \
    food.cpp \
    network.cpp \
    genome.cpp \
    geneticalgorithm.cpp \
    global.cpp \
    species.cpp

HEADERS  += mainwindow.h \
    creature.h \
    neuron.h \
    food.h \
    global.h \
    network.h \
    genome.h \
    geneticalgorithm.h \
    species.h

FORMS    += mainwindow.ui

CONFIG += c++11
