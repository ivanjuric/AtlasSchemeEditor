#include "libraryfile.h"
#include "memory.h"
#include "visualrectangle.h"
#include "visualtext.h"
#include "visualcircle.h"
#include "regularbusview.h"
#include "componentmodel.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QFile>
#include <qjsonarray.h>

// Represents initial library file that contains all informations
// needed to prepare main screen and whole system to work with
// specified computer system.
LibraryFile::LibraryFile(QString filepath)
{
    if(loadJson(filepath))
        this->filePath = filepath;
    else
        this->filePath = "";
}

// Loads JSON from file and parses it and assigns to LibraryFile class properties
bool LibraryFile::loadJson(QString filepath)
{
    // Try to open file and read it
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    // Parse json file
    QString data = file.readAll();
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
        c->setComdelFile(obj["COMDELfile"].toString());

        // Load component views
        loadComponentViews(obj["view"].toArray(), c);

        // Load visual pin list
        loadComponentPins(obj["visualPinList"].toArray(), c);

        componentList.append(c);
    }
}
void LibraryFile::loadComponentViews(QJsonArray views, ComponentModel *component)
{
    foreach (QJsonValue viewVal, views) {
       QJsonObject viewObject = viewVal.toObject();

       // Determine view type
       QString viewType = viewObject["viewType"].toString();

       // Parse position values: x and y
       int x = viewObject["x"].toInt();
       int y = viewObject["y"].toInt();
       QColor mainColor = getColor(viewObject["mainColor"].toString());

       if(viewType == "rectangle"){
           VisualRectangle *rectangle = new VisualRectangle(x,y,mainColor);
           rectangle->width = viewObject["width"].toInt();
           rectangle->height = viewObject["height"].toInt();
           QString colorName = viewObject["fillColor"].toString();
           rectangle->fillColor = QColor::isValidColor(colorName) ? QColor(colorName) : QColor(Qt::white);
           component->addVisualElement(rectangle);
       }
       else if(viewType == "circle"){
           VisualCircle *circle = new VisualCircle(x,y,mainColor);
           circle->radius = viewObject["radius"].toInt();
           QString colorName = viewObject["fillColor"].toString();
           circle->fillColor = QColor::isValidColor(colorName) ? QColor(colorName) : QColor(Qt::white);
           component->addVisualElement(circle);
       }
       else if(viewType == "text"){
           VisualText *text = new VisualText(x,y,mainColor);
           text->text = viewObject["string"].toString();
           component->addVisualElement(text);
       }
    }
}
void LibraryFile::loadComponentPins(QJsonArray pins, ComponentModel *component)
{
    int uid = 0;
    foreach (QJsonValue pinVal, pins)
    {
        QJsonObject pinObject = pinVal.toObject();

        QJsonObject pinViewObject = pinObject["view"].toObject();

        // Load PinModel properties
        PinModel *pin = new PinModel(uid++);
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

        component->addPin(pin);
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
        bus->uid = randId++;
        bus->ID = busObject["ID"].toString();
        loadBusLines(busObject["busLines"].toArray(), bus);

        // Fill regular bus properties, else fill automatic bus list
        if(regularBusType)
        {
            //RegularBus *regBus = ((RegularBus*)bus);
            ((RegularBus*)bus)->title = busObject["title"].toString();
            ((RegularBus*)bus)->tooltip = busObject["tooltip"].toString();
            ((RegularBus*)bus)->iconFile = busObject["iconFile"].toString();
            ((RegularBus*)bus)->minInstances = busObject["minInstances"].toInt();
            ((RegularBus*)bus)->maxInstances = busObject["maxInstances"].toInt();

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

        bus->busLines.append(line);
    }
}
void LibraryFile::loadBusView(QJsonObject view, RegularBus *bus)
{
    bus->setOrientation(view["lineOrientation"].toString());
    bus->setLineColor(getColor(view["lineColor"].toString()));
    bus->setLineThickness(view["lineThickness"].toInt());
    bus->setLineLength(view["lineLength"].toInt());
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

// -------------------  STARI KOD -----------------
Bus *LibraryFile::GetBusByUniqueId(int uid)
{
    foreach(RegularBus* bus, this->regularBuses)
    {
       if(bus->uid == uid)
           return bus;
    }
    return 0;
}


