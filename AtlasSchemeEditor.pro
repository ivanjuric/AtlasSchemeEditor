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
    componentview.cpp \
    visualcomponentelement.cpp \
    visualrectangle.cpp \
    visualtext.cpp \
    visualcircle.cpp \
    pinview.cpp \
    addressspace.cpp \
    messages.cpp \
    connection.cpp \
    bus.cpp \
    busline.cpp \
    regularbus.cpp \
    automaticbus.cpp \
    pinmodel.cpp \
    componentmodel.cpp \
    regularbusview.cpp \
    attribute.cpp \
    rulepopup.cpp \
    pinrulepopup.cpp \
    regularbusconnectionrule.cpp \
    automaticbusconnectionrule.cpp \
    automaticbusconnectionrulecomponent.cpp \
    popupnumericvalue.cpp \
    popupenumeratedvalue.cpp \
    toolboxbutton.cpp \
    graphicsview.cpp \
    rulecheck.cpp

HEADERS  += schemeeditormainwindow.h \
    libraryfile.h \
    componentview.h \
    visualcomponentelement.h \
    visualrectangle.h \
    visualtext.h \
    visualcircle.h \
    pinview.h \
    addressspace.h \
    messages.h \
    connection.h \
    bus.h \
    busline.h \
    regularbus.h \
    automaticbus.h \
    Enums.h \
    pinmodel.h \
    componentmodel.h \
    regularbusview.h \
    attribute.h \
    rulepopup.h \
    pinrulepopup.h \
    regularbusconnectionrule.h \
    automaticbusconnectionrule.h \
    automaticbusconnectionrulecomponent.h \
    popupnumericvalue.h \
    popupenumeratedvalue.h \
    toolboxbutton.h \
    graphicsview.h \
    rulecheck.h

FORMS    += schemeeditormainwindow.ui \
    popupnumericvalue.ui \
    popupenumeratedvalue.ui

RESOURCES += \
    images.qrc
