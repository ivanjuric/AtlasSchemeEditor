#-------------------------------------------------
#
# Project created by QtCreator 2014-03-29T14:29:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AtlasSchemeEditor
TEMPLATE = app


SOURCES += main.cpp\
        schemeeditormainwindow.cpp \
    basecomponent.cpp \
    pin.cpp \
    connection.cpp \
    guibasecomponent.cpp \
    graphwidget.cpp

HEADERS  += schemeeditormainwindow.h \
    basecomponent.h \
    pin.h \
    connection.h \
    guibasecomponent.h \
    graphwidget.h

FORMS    += schemeeditormainwindow.ui
