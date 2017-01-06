#-------------------------------------------------
#
# Project created by QtCreator 2016-01-09T17:58:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CodeBreakerGUI
TEMPLATE = app


SOURCES += main.cpp\
        CodeBreakerMain.cpp \
    Bifid.cpp \
    Globals.cpp \
    GuessCipher.cpp \
    GlobalSettings.cpp \
    Vigenere.cpp \
    SimpleSubstitution.cpp \
    RailFence.cpp \
    Trifid.cpp \
    BinaryConvert.cpp \
    Space.cpp \
    FourSquare.cpp \
    Amsco.cpp \
    ColumnView.cpp \
    columnartransposition.cpp \
    Playfair.cpp \
    PolybiusSquare.cpp \
    Cadenus.cpp \
    hill.cpp \
    hill.cpp

HEADERS  += CodeBreakerMain.h \
    Bifid.h \
    Bigrams.h \
    Globals.h \
    Quadgrams.h \
    Trigrams.h \
    GuessCipher.h \
    GlobalSettings.h \
    Vigenere.h \
    SimpleSubstitution.h \
    Cipher.h \
    RailFence.h \
    Trifid.h \
    BinaryConvert.h \
    Space.h \
    hashtable.h \
    linkedlist.h \
    FourSquare.h \
    Amsco.h \
    ColumnView.h \
    columnartransposition.h \
    Playfair.h \
    PolybiusSquare.h \
    Cadenus.h \
    hill.h \
    hill.h

FORMS    += CodeBreakerMain.ui \
    GuessCipher.ui \
    GlobalSettings.ui \
    Vigenere.ui \
    SimpleSubstitution.ui \
    Bifid.ui \
    RailFence.ui \
    Trifid.ui \
    BinaryConvert.ui \
    FourSquare.ui \
    Amsco.ui \
    ColumnView.ui \
    columnartransposition.ui \
    Playfair.ui \
    PolybiusSquare.ui \
    Cadenus.ui \
    hill.ui \
    hill.ui

CONFIG += c++11 \ console
QT += sql
