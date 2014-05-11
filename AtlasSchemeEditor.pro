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
    libraryfile.cpp \
    editorgraphicsview.cpp \
    componentmodel.cpp \
    componentview.cpp \
    visualcomponentelement.cpp \
    visualrectangle.cpp \
    visualtext.cpp \
    visualcircle.cpp

HEADERS  += schemeeditormainwindow.h \
    libraryfile.h \
    editorgraphicsview.h \
    componentmodel.h \
    componentview.h \
    visualcomponentelement.h \
    visualrectangle.h \
    visualtext.h \
    visualcircle.h

FORMS    += schemeeditormainwindow.ui

RESOURCES += \
    images.qrc
