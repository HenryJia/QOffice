#-------------------------------------------------
#
# Project created by QtCreator 2014-08-11T12:18:24
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Spreadsheet
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cell.cpp \
    gotocelldialog.cpp \
    sortdialog.cpp \
    spreadsheet.cpp \
    spreadsheetcompare.cpp \
    findandreplacewidget.cpp

HEADERS  += mainwindow.h \
    cell.h \
    gotocelldialog.h \
    sortdialog.h \
    spreadsheet.h \
    spreadsheetcompare.h \
    findandreplacewidget.h

FORMS += \
    gotocelldialog.ui \
    sortdialog.ui \
    findandreplacewidget.ui

OTHER_FILES += \
    TODO

RESOURCES += \
    spreadsheet.qrc
