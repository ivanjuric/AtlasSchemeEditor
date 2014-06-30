#include "libraryfile.h"
#include "memory.h"
#include "visualrectangle.h"
#include "visualtext.h"
#include "visualcircle.h"
#include "regularbusview.h"
#include "componentmodel.h"
#include <QJsonDocument>
#include <QJsonObject>
#include "attribute.h"
#include "checkconnection.h"
#include "qstring.h"
#include <QFile>
#include <qjsonarray.h>

// Represents initial library file that contains all informations
// needed to prepare main screen and whole system to work with
// specified computer system.
LibraryFile::LibraryFile()
{
    libraryDirPath = QDir::currentPath() + "/library";
}

// Loads JSON from file and parses it and assigns to LibraryFile class properties
bool LibraryFile::loadJson(QString filepath)
{
    // Try to open file and read it
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    // Copy library file to root directory
    QDir rootDir(libraryDirPath);
    if(!rootDir.exists())
        rootDir.mkdir(libraryDirPath);

    QFileInfo loadedInfo(file);
    QFileInfo info(libraryDirPath, loadedInfo.fileName());
    if(info.exists())
    {
        QFile newFile(info.absoluteFilePath());
        if(newFile.remove())
        {
            //QString s = file.readAll();
            file.copy(loadedInfo.absoluteFilePath(), info.absoluteFilePath());

        }
    }
    else
    {
        file.copy(loadedInfo.absoluteFilePath(), info.absoluteFilePath());
    }

    QFile libFile(info.absoluteFilePath());
    // Parse json file
    if(!libFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    libraryFileName = info.fileName();
    QString data = libFile.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(data.toUtf8()));

    // Get JSON root object
    QJsonObject root = doc.object();

    // Load basic information
    libraryTitle = root["libraryTitle"].toString();
    libraryInfo = root["libraryInfo"].toString();
    comdelDirectory = root["comdelDirectory"].toString();
    loadComdelHeader(root["comdelHeader"].toArray());

    // Load list of address spaces
    loadAddressSpaceList(root["addressSpaceList"].toArray());

    // Load messages
    loadMessages(root["messages"].toObject());

    // Load all components and its children
    loadComponents(root["componentList"].toArray());

    // Load automatic and regular buses
    loadBuses(root["regularBusList"].toArray(), true);
    loadBuses(root["automaticBusList"].toArray());

    // Load connection rules
    loadRegularBusConnectionRules(root["regularBusConnectionRules"].toArray());
    loadAutomaticBusConnectionRules(root["automaticBusConnectionRules"].toArray());

    return true;
}

// Methods for loading basic information from json library file
void LibraryFile::loadComdelHeader(QJsonArray headerLines)
{
    foreach (QJsonValue line, headerLines)
    {
        comdelHeader.append(line.toString());
    }
}
void LibraryFile::loadAddressSpaceList(QJsonArray addressSpaces)
{
    foreach (QJsonValue adr, addressSpaces){
        QJsonObject obj = adr.toObject();

        AddressSpace *addressSpace = new AddressSpace();
        addressSpace->id = obj["ID"].toString();
        addressSpace->min = obj["min"].toString();
        addressSpace->max = obj["max"].toString();

        addressSpaceList.append(addressSpace);
    }
}
void LibraryFile::loadMessages(QJsonObject messagesObj)
{
    messages.OK = messagesObj["OK"].toString();
    messages.Yes = messagesObj["Yes"].toString();
    messages.No = messagesObj["No"].toString();
    messages.Cancel = messagesObj["Cancel"].toString();
    messages.noneBusLine = messagesObj["noneBusLine"].toString();
    messages.noneValue = messagesObj["noneValue"].toString();
    messages.generalPinNotConnected = messagesObj["generalPinNotConnected"].toString();
}

// Component loader and component pins and views loader methods
void LibraryFile::loadComponents(QJsonArray compArray)
{
    foreach (QJsonValue comp, compArray)
    {
        QJsonObject obj = comp.toObject();

        // Create component model object and load data from libraray
        ComponentModel *c = new ComponentModel();
        c->setId(obj["ID"].toString());
        c->setTitle(obj["title"].toString());
        c->setInstanceNameBase(obj["instanceNameBase"].toString());
        c->setIconFile(obj["iconFile"].toString());
        c->setTooltip(obj["tooltip"].toString());
        c->setMinInstances(obj["minInstances"].toInt());
        c->setMaxInstances(obj["maxInstances"].toInt());
        c->setComdelFile(obj["comdelFile"].toString());

        // Load instantiation parameters
        foreach(QJsonValue s, obj["instantiationParameters"].toArray())
            c->addInstantiationParameter(s.toString());

        // Load component views
        loadComponentViews(obj["view"].toArray(), c);

        // Load visual pin list
        loadComponentPins(obj["visualPinList"].toArray(), c);

        // Load component attributes
        loadAttributes(obj["attributes"].toArray(), c);

        componentList.append(c);
    }
}
void LibraryFile::loadComponentViews(QJsonArray views, ComponentModel *component)
{
    foreach (QJsonValue viewVal, views)
    {
       QJsonObject viewObject = viewVal.toObject();

       // Determine view type
       QString viewType = viewObject["viewType"].toString();

       // Parse position values: x and y
       int x = viewObject["x"].toInt();
       int y = viewObject["y"].toInt();
       QColor mainColor = getColor(viewObject["mainColor"].toString());

       if(viewType == "rectangle")
       {
           VisualRectangle *rectangle = new VisualRectangle(x,y,mainColor);
           rectangle->width = viewObject["width"].toInt();
           rectangle->height = viewObject["height"].toInt();
           QString colorName = viewObject["fillColor"].toString();
           rectangle->fillColor = QColor::isValidColor(colorName) ? QColor(colorName) : QColor(Qt::white);
           rectangle->lineThickness = viewObject["lineThickness"].toInt();
           component->addVisualElement(rectangle);
       }
       else if(viewType == "circle")
       {
           VisualCircle *circle = new VisualCircle(x,y,mainColor);
           circle->radius = viewObject["radius"].toInt();
           QString colorName = viewObject["fillColor"].toString();
           circle->fillColor = QColor::isValidColor(colorName) ? QColor(colorName) : QColor(Qt::white);
           circle->lineThickness = viewObject["lineThickness"].toInt();
           component->addVisualElement(circle);
       }
       else if(viewType == "text")
       {
           VisualText *text = new VisualText(x,y,mainColor);
           text->showInstanceName = false;
           text->text = viewObject["string"].toString();
           component->addVisualElement(text);
       }
       else if(viewType == "instanceName")
       {
           VisualText *text = new VisualText(x,y,mainColor);
           text->showInstanceName = true;
           component->addVisualElement(text);
       }
    }
}
void LibraryFile::loadComponentPins(QJsonArray pins, ComponentModel *component)
{
    foreach (QJsonValue pinVal, pins)
    {
        QJsonObject pinObject = pinVal.toObject();

        QJsonObject pinViewObject = pinObject["view"].toObject();

        // Load PinModel properties
        PinModel *pin = new PinModel();
        pin->setId(pinObject["ID"].toString());
        pin->setTitle(pinObject["title"].toString());
        pin->setTooltip(pinObject["tooltip"].toString());
        pin->setShape(getShapeFromString(pinViewObject["shape"].toString()));
        pin->setX(pinViewObject["x"].toInt());
        pin->setY(pinViewObject["y"].toInt());
        int dimension = pinViewObject["dimension"].toInt();
        pin->setWidth(dimension);
        pin->setHeight(dimension);
        pin->setSide(getSideFromString(pinViewObject["side"].toString()));
        pin->setLineColor(getColor(pinViewObject["lineColor"].toString()));
        pin->setLineColorConnected(getColor(pinViewObject["lineColorConnected"].toString()));
        pin->setFillColor(getColor(pinViewObject["fillColor"].toString()));
        pin->setFillColorConnected(getColor(pinViewObject["fillColorConnected"].toString()));

        loadComponentPinsCheckConnection(pinObject["check_connection"].toObject(), pin);
        component->addPin(pin);
    }
}
void LibraryFile::loadComponentPinsCheckConnection(QJsonObject checkConnection, PinModel *pinModel)
{
    CheckConnection *cc = new CheckConnection();

    QString typeString = checkConnection["type"].toString();
    CheckConnectionTypeEnum type;
    if(typeString == "connect_on_demand")
        type = CheckConnectionTypeEnum::ConnectOnDemand;
    else if(typeString == "check_only")
        type = CheckConnectionTypeEnum::CheckOnly;

    cc->setType(type);
    cc->setMessage(checkConnection["message"].toString());

    QJsonObject autoConn = checkConnection["automatic_connection"].toObject();
    if(!autoConn.isEmpty())
    {
        AutomaticConnection *automaticConnection = new AutomaticConnection();
        automaticConnection->setPopupOptionConnect(autoConn["popup_option_connect"].toString());
        automaticConnection->setPopupOptionContinue(autoConn["popup_option_continue"].toString());
        automaticConnection->setContextMenuItemText(autoConn["contextMenuItemText"].toString());
        automaticConnection->setAutomaticBus(autoConn["automaticBus"].toString());
        foreach(QJsonValue pin, autoConn["pinInstantiationList"].toArray())
        {
            automaticConnection->addToPinInstantiationList(pin.toString());
        }

        cc->setAutomaticConnection(automaticConnection);
    }
    else
    {
        cc->setAutomaticConnection(0);
    }
    pinModel->setCheckConnection(cc);
}

void LibraryFile::loadAttributes(QJsonArray attributes, ComponentModel *component)
{
    foreach (QJsonValue attrVal, attributes)
    {
       QJsonObject attrObject = attrVal.toObject();

       Attribute *attr = new Attribute();
       attr->setId(attrObject["ID"].toString());
       attr->setTitle(attrObject["title"].toString());
       attr->setPopupText(attrObject["popupText"].toString());
       attr->setPopupType(getPopupTypeFromString(attrObject["popup"].toString()));
       attr->setDefaultValue(attrObject["default"].toInt());

       QJsonArray enums = attrObject["enumerated"].toArray();
       if(enums.count() > 0)
           loadAttributeEnumeratedValues(enums, attr);

       QJsonArray rules = attrObject["rules"].toArray();
       if(rules.count() > 0)
           loadRuleChecks(rules, attr);

       component->addAttribute(attr);

    }
}
void LibraryFile::loadAttributeEnumeratedValues(QJsonArray enumeratedValues, Attribute *attribute)
{
    foreach (QJsonValue enumVal, enumeratedValues)
    {
       QJsonObject enumValObject = enumVal.toObject();
       QString name = enumValObject["text"].toString();
       int value = enumValObject["value"].toInt();

       attribute->addEnumeratedValue(name, value);
    }
}
void LibraryFile::loadRuleChecks(QJsonArray ruleValues, Attribute *attribute)
{
    foreach (QJsonValue ruleVal, ruleValues)
    {
        QJsonObject ruleObject = ruleVal.toObject();
        RuleCheck *rule = new RuleCheck();
        rule->setType(getRuleCheckTypeFromString(ruleObject["type"].toString()));
        rule->setCondition(getRuleCheckConditionFromString(ruleObject["condition"].toString()));
        foreach(QJsonValue par, ruleObject["parameters"].toArray())
        {
            rule->addParameter(par.toString());
        }
        rule->setErrorMessage(ruleObject["errorMessage"].toString());
        attribute->addRule(rule);
    }
}

// Method for loading both automatic (regularBusType=false) and regular (regularBusType=true) buses.
void LibraryFile::loadBuses(QJsonArray busList, bool regularBusType)
{
    if(regularBusType)
        regularBuses.clear();
    else
        automaticBuses.clear();

    foreach(QJsonValue busVal, busList)
    {
        QJsonObject busObject = busVal.toObject();

        // Create bus, type depends on bool parameter
        Bus *bus;
        if(regularBusType)
            bus = new RegularBus();
        else
            bus = new AutomaticBus();

        // Fill common properties for both bus types
        bus->setId(busObject["ID"].toString());
        loadBusLines(busObject["busLines"].toArray(), bus);

        // Fill regular bus properties, else fill automatic bus list
        if(regularBusType)
        {
            //RegularBus *regBus = ((RegularBus*)bus);
            ((RegularBus*)bus)->setTitle(busObject["title"].toString());
            ((RegularBus*)bus)->setTooltip(busObject["tooltip"].toString());
            ((RegularBus*)bus)->setIconFile(busObject["iconFile"].toString());
            ((RegularBus*)bus)->setMinInstances(busObject["minInstances"].toInt());
            ((RegularBus*)bus)->setMaxInstances(busObject["maxInstances"].toInt());

            loadBusView(busObject["view"].toObject(), (RegularBus*)bus);
            regularBuses.append(((RegularBus*)bus));
        }
        else
        {
            automaticBuses.append((AutomaticBus*)bus);
        }
    }
}
void LibraryFile::loadBusLines(QJsonArray busLines, Bus *bus)
{
    foreach(QJsonValue lineVal, busLines)
    {
        QJsonObject lineObject = lineVal.toObject();

        //Create line and pass obligatory fields
        BusLine *line = new BusLine(lineObject["ID"].toString(), lineObject["size"].toInt());

        // Fill with optional fields
        line->setType(lineObject["type"].toString());
        line->setTerminateWith(lineObject["terminate_with"].toInt());
        line->setIfUnterminated(lineObject["if_unterminated"].toInt());

        bus->addBusLine(line);
    }
}
void LibraryFile::loadBusView(QJsonObject view, RegularBus *bus)
{
    bus->setOrientation(getOrientationFromString(view["lineOrientation"].toString()));
    bus->setLineColor(getColor(view["lineColor"].toString()));
    bus->setLineThickness(view["lineThickness"].toInt());
    bus->setLineLength(view["lineLength"].toInt());
}

// Load regular bus connection rules
void LibraryFile::loadRegularBusConnectionRules(QJsonArray rulesArray)
{
    foreach(QJsonValue ruleVal, rulesArray)
    {
        QJsonObject ruleObject = ruleVal.toObject();

        RegularBusConnectionRule *rule = new RegularBusConnectionRule();
        rule->setComponentId(ruleObject["component"].toString());
        rule->setComponentVisualPinId(ruleObject["componentVisualPin"].toString());
        rule->setBusId(ruleObject["bus"].toString());
        foreach(QJsonValue pin, ruleObject["pinInstantiationList"].toArray())
        {
            rule->addPinIdToInstantiationList(pin.toString());
        }
        foreach(QJsonValue term, ruleObject["connectionTerminates"].toArray())
        {
            rule->addToConnectionTerminates(term.toString());
        }
        loadRulePopup(ruleObject["popup"].toArray(), rule);

        regularBusConnectionRules.append(rule);
    }
}
void LibraryFile::loadRulePopup(QJsonArray popupArray, RegularBusConnectionRule *rule)
{
    foreach(QJsonValue popupVal, popupArray)
    {
        QJsonObject popupObject = popupVal.toObject();

        RulePopup *popup = new RulePopup();
        popup->setId(popupObject["ID"].toString());
        popup->setContextMenuItem(popupObject["contextMenuItem"].toString());
        popup->setOpen(getPopupTypeFromString(popupObject["open"].toString()));
        popup->setMessage(popupObject["message"].toString());
        popup->setPinHeaderText(popupObject["pinHeaderText"].toString());
        popup->setBusLineHeaderText(popupObject["busLineHeaderText"].toString());
        loadRulePopupPinList(popupObject["pinList"].toArray(), popup);

        rule->addPopup(popup);
    }
}
void LibraryFile::loadRulePopupPinList(QJsonArray pinArray, RulePopup *popup)
{
    foreach(QJsonValue pinVal, pinArray)
    {
        QJsonObject pinObject = pinVal.toObject();

        PinRulePopup *pin = new PinRulePopup();
        pin->setId(pinObject["ID"].toString());
        pin->setTitle(pinObject["title"].toString());
        pin->setDefaultPin(pinObject["default"].toString());
        foreach (QJsonValue busLineName, pinObject["busLineList"].toArray())
        {
            pin->addBusLineToList(busLineName.toString());
        }

        popup->addPinRulePopupToPinList(pin);
    }
}

// Load automatic bus connection rules
void LibraryFile::loadAutomaticBusConnectionRules(QJsonArray rulesArray)
{
    foreach(QJsonValue ruleVal, rulesArray)
    {
        QJsonObject ruleObject = ruleVal.toObject();

        AutomaticBusConnectionRule *rule = new AutomaticBusConnectionRule();
        rule->setBusId(ruleObject["automaticBus"].toString());
        AutomaticBusConnectionRuleComponent *first = loadAutomaticBusConnectionRuleComponent(ruleObject["firstComponent"].toObject());
        AutomaticBusConnectionRuleComponent *second = loadAutomaticBusConnectionRuleComponent(ruleObject["secondComponent"].toObject());

        rule->setFirstComponent(first);
        rule->setSecondComponent(second);

        automaticBusConnectionRules.append(rule);
    }
}
AutomaticBusConnectionRuleComponent* LibraryFile::loadAutomaticBusConnectionRuleComponent(QJsonObject compObj)
{
    AutomaticBusConnectionRuleComponent *comp = new AutomaticBusConnectionRuleComponent();
    comp->setComponentId(compObj["component"].toString());
    comp->setComponentVisualPinId(compObj["componentVisualPin"].toString());
    foreach(QJsonValue pin, compObj["pinInstantiationList"].toArray())
    {
        comp->addPinIdToInstantiationList(pin.toString());
    }
    return comp;
}

RuleCheckTypeEnum LibraryFile::getRuleCheckTypeFromString(QString type)
{
    RuleCheckTypeEnum t;
    if(type.toLower() == "error")
        t = RuleCheckTypeEnum::Error;
    else if(type.toLower() == "warning")
        t = RuleCheckTypeEnum::Warning;
    return t;
}
RuleCheckConditionEnum LibraryFile::getRuleCheckConditionFromString(const QString condition)
{
    RuleCheckConditionEnum c;

    if(condition == "divisible_by_power_of_2")
        c = RuleCheckConditionEnum::DivisibleByPowerOf2;
    else if(condition == "greater_or_equal")
        c = RuleCheckConditionEnum::GreaterOrEqual;
    else if(condition == "less_or_equal")
        c = RuleCheckConditionEnum::LessOrEqual;
    else if(condition == "unique_in_address_space")
        c = RuleCheckConditionEnum::UniqueInAddressSpace;
    else if(condition == "divisible_by")
        c = RuleCheckConditionEnum::DivisibleBy;
    else if(condition == "starts_in_address_space")
        c = RuleCheckConditionEnum::StartsInAddressSpace;
    else if(condition == "ends_in_address_space")
        c = RuleCheckConditionEnum::EndsInAddressSpace;
    else if(condition == "in_range")
        c = RuleCheckConditionEnum::InRange;

    return c;
}

OrientationEnum LibraryFile::getOrientationFromString(QString orientation)
{
    if(orientation == "vertical")
        return OrientationEnum::Vertical;
    else
        return OrientationEnum::Horizontal;
}
PinTypeEnum LibraryFile::getShapeFromString(QString shapeString)
{
    PinTypeEnum shape;

    if(shapeString.toLower() == "in")
        shape = PinTypeEnum::In;
    else if(shapeString.toLower() == "out")
        shape = PinTypeEnum::Out;
    else if(shapeString.toLower() == "square")
        shape = PinTypeEnum::Square;
    else if(shapeString.toLower() == "circle")
        shape = PinTypeEnum::Circle;
    else if(shapeString.toLower() == "inout")
        shape = PinTypeEnum::InOut;
    else if(shapeString.toLower() == "squarein")
        shape = PinTypeEnum::SquareIn;
    else if(shapeString.toLower() == "squareout")
        shape = PinTypeEnum::SquareOut;
    else if(shapeString.toLower() == "squareinout")
        shape = PinTypeEnum::SquareInOut;
    else
        shape = PinTypeEnum::BusPin;

    return shape;
}

PinSideEnum LibraryFile::getSideFromString(QString side)
{
    if(side.toLower() == "left")
        return PinSideEnum::Left;
    else if(side.toLower() == "right")
        return PinSideEnum::Right;
    else
        return PinSideEnum::None;
}
// Parse main color (line or text color). Allowed formats are hex (#000000) or name ("black")
QColor LibraryFile::getColor(QString colorName, bool fill)
{
    return QColor::isValidColor(colorName) ? QColor(colorName) : (fill ? QColor(Qt::white) : QColor(Qt::black));
}
ComponentModel *LibraryFile::getComponentById(QString id)
{
    foreach (ComponentModel *c, this->componentList)
    {
        if(c->id() == id)
            return c;
    }
    return 0;
}
PopupTypeEnum LibraryFile::getPopupTypeFromString(QString popupType)
{
    PopupTypeEnum type = (PopupTypeEnum)0;
    if(popupType.toLower() == "automatic")
        type = PopupTypeEnum::Automatic;
    else if(popupType.toLower() == "on_demand")
        type = PopupTypeEnum::OnDemand;

    return type;
}
AutomaticBus* LibraryFile::getAutomaticBusById(QString id)
{
    foreach (AutomaticBus *bus, automaticBuses)
    {
        if(bus->id() == id)
            return bus;
    }
    return 0;
}
Bus *LibraryFile::GetBusByUniqueId(QString id)
{
    foreach(RegularBus* bus, this->regularBuses)
    {
       if(bus->id() == id)
           return bus;
    }
    return 0;
}


