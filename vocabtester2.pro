#-------------------------------------------------
#
# Project created by QtCreator 2011-11-12T03:01:32
#
#-------------------------------------------------

QT       += core gui

TARGET = vocabtester2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        flashcard.cpp \
    cardpack.cpp \
    svfileops.cpp \
    cardset.cpp

HEADERS  += mainwindow.h \
    cardpack.h \
    flashcard.h \
    svfileops.h \
    cardset.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
