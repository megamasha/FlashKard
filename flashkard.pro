#-------------------------------------------------
#
# Project created by QtCreator 2011-11-12T03:01:32
#
#-------------------------------------------------

QT       += core gui sql xml widgets

TARGET = flashkard
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        flashcard.cpp \
    cardpack.cpp \
    svfileops.cpp \
    cardset.cpp \
    flashcardwindow.cpp \
    popupwindow.cpp \
    resultswindow.cpp \
    editorwindow.cpp \
    databasewindow.cpp \
    carddatabasemodel.cpp \
    statswindow.cpp \
    fmlhandler.cpp \
    carddatabaseproxymodel.cpp \
    preferenceswindow.cpp \
    flashkardtrayicon.cpp

HEADERS  += mainwindow.h \
    cardpack.h \
    flashcard.h \
    svfileops.h \
    cardset.h \
    flashcardwindow.h \
    popupwindow.h \
    resultswindow.h \
    editorwindow.h \
    databasewindow.h \
    carddatabasemodel.h \
    statswindow.h \
    fmlhandler.h \
    carddatabaseproxymodel.h \
    preferenceswindow.h \
    flashkardtrayicon.h

FORMS    += mainwindow.ui \
    flashcardwindow.ui \
    resultswindow.ui \
    editorwindow.ui \
    databasewindow.ui \
    statswindow.ui \
    preferenceswindow.ui

RESOURCES += \
    resources.qrc
