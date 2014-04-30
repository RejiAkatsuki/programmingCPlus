#-------------------------------------------------
#
# Project created by QtCreator 2013-12-09T11:39:37
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LibraryRecord
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    library.cpp \
    collection.cpp \
    borrowergui.cpp \
    collection_list.cpp \
    copy.cpp \
    copygui.cpp \
    readwrite.cpp

HEADERS  += mainwindow.h \
    library.h \
    collection.h \
    copy.h \
    collection_list.h \
    borrowergui.h \
    borrower.h \
    borrower_list.h \
    database.h \
    copygui.h \
    readwrite.h

FORMS    += mainwindow.ui \
    borrower.ui \
    library.ui \
    copygui.ui

RESOURCES += \
    BackgroundImage.qrc
