#include "qstring.h"
#include <QVector>
#include "componentview.h"
#include "componentmodel.h"
#include "addressspace.h"
#include "messages.h"
#include "regularbus.h"
#include "automaticbus.h"
#include "regularbusview.h"
#include "regularbusconnectionrule.h"
#include "automaticbusconnectionrule.h"

#ifndef LIBRARYFILE_H
#define LIBRARYFILE_H

#pragma once


class LibraryFile
{
public:
    LibraryFile(QString filepath);

    Bus *GetBusByUniqueId(QString id);


    // Properties
    QString filePath;
    QString libraryTitle;
    QString libraryInfo;
    QVector<QString> comdelHeader;
    QString comdelDirectory;
    QVector<AddressSpace*> addressSpaceList;
    QVector<ComponentModel*> componentList;
    QVector<RegularBus*> regularBuses;
    QVector<AutomaticBus*> automaticBuses;
    QVector<RegularBusConnectionRule*> regularBusConnectionRules;
    QVector<AutomaticBusConnectionRule*> automaticBusConnectionRules;
    Messages messages;

    // Methods
    ComponentModel *getComponentById(QString id);
    AutomaticBus* getAutomaticBusById(QString id);

private:
    int randId;

    bool loadJson(QString filepath);
    void loadComdelHeader(QJsonArray headerLines);
    void loadAddressSpaceList(QJsonArray addressSpaces);
    void loadMessages(QJsonObject messagesObj);
    void loadComponents(QJsonArray compArray);
    void loadComponentViews(QJsonArray views, ComponentModel *component);
    void loadComponentPins(QJsonArray pins, ComponentModel *component);
    void loadAttributes(QJsonArray attributes, ComponentModel *component);
    void loadAttributeEnumeratedValues(QJsonArray enumeratedValues, Attribute *attribute);
    void loadBuses(QJsonArray busList, bool regularBusType = false);
    void loadBusLines(QJsonArray busLines, Bus *bus);
    void loadBusView(QJsonObject view, RegularBus *bus);
    void loadRegularBusConnectionRules(QJsonArray rulesArray);
    void loadRulePopup(QJsonArray popupArray, RegularBusConnectionRule *rule);
    void loadRulePopupPinList(QJsonArray pinArray, RulePopup *popup);
    void loadAutomaticBusConnectionRules(QJsonArray rulesArray);
    AutomaticBusConnectionRuleComponent *loadAutomaticBusConnectionRuleComponent(QJsonObject compObj);
    QColor getColor(QString colorName, bool fill = false);
    PinTypeEnum getShapeFromString(QString shapeString);
    PinSideEnum getSideFromString(QString side);
    OrientationEnum getOrientationFromString(QString orientation);
    PopupTypeEnum getPopupTypeFromString(QString popupType);
};

#endif // LIBRARYFILE_H
