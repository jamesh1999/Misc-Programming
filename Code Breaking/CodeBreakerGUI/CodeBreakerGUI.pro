#-------------------------------------------------
#
# Project created by QtCreator 2015-04-03T17:17:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CodeBreakerGUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Amsco.cpp \
    Bifid.cpp \
    BinaryConvert.cpp \
    Cadenus.cpp \
    ColumnarTransposition.cpp \
    FourSquare.cpp \
    Globals.cpp \
    Hill.cpp \
    Playfair.cpp \
    PolybiusSquare.cpp \
    RailFence.cpp \
    SimpleSubstitution.cpp \
    Trifid.cpp \
    Vigenere.cpp \
    decrypt.cpp \
    globalsettings.cpp \
    guesscipher.cpp

HEADERS  += mainwindow.h \
    decrypt.h \
    Amsco.h \
    Bifid.h \
    Bigrams.h \
    BinaryConvert.h \
    Cadenus.h \
    ColumnarTransposition.h \
    FourSquare.h \
    Globals.h \
    Hill.h \
    Playfair.h \
    PolybiusSquare.h \
    Quadgrams.h \
    RailFence.h \
    SimpleSubstitution.h \
    Trifid.h \
    Trigrams.h \
    Vigenere.h \
    globalsettings.h \
    guesscipher.h

FORMS    += mainwindow.ui \
    globalsettings.ui \
    guesscipher.ui

DISTFILES +=
CONFIG += c++11
