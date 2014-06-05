#include "libraryfile.h"
#include "memory.h"
#include "visualrectangle.h"
#include "visualtext.h"
#include "visualcircle.h"
#include "busview.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QFile>
#include <qjsonarray.h>

// Represents initial library file that contains all informations
// needed to prepare main screen and whole system to work with
// specified computer system.
LibraryFile::LibraryFile(QString filepath){
    if(loadJson(filepath))
        this->filePath = filepath;
}

// Loads JSON from file and parses it and assigns to LibraryFile class properties
bool LibraryFile::loadJson(QString filepath){
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

    // Generate random id for components and buses
    randId = rand();

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
    foreach (QJsonValue line, headerLines){
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
    foreach (QJsonValue comp, compArray){
        QJsonObject obj = comp.toObject();

        QString id = obj["ID"].toString();
        QString title = obj["title"].toString();
        QString tooltip = obj["tooltip"].toString();
        int minInstances = obj["minInstances"].toInt();
        int maxInstances = obj["maxInstances"].toInt();
        QString COMDELfile = obj["COMDELfile"].toString();

        QJsonObject nulica = obj["attributes"].toObject();

        if(obj["attributes"].isNull()){
            nulica = obj;
        }
        if(obj["maxInstances"].isNull())
            minInstances = 0;

        ComponentModel *c = new ComponentModel();

        c->uid = randId++;
        c->id = obj["ID"].toString();
        c->title = obj["title"].toString();
        c->instanceNameBase = obj["instanceNameBase"].toString();
        c->iconFile = obj["iconFile"].toString();
        c->tooltip = obj["tooltip"].toString();
        c->minInstances = obj["minInstances"].toInt();
        c->maxInstances = obj["maxInstances"].toInt();
        c->comdelFile = obj["comdelFile"].toString();

        // Load component views
        loadComponentViews(obj["view"].toArray(), c);

        // Load visual pin list
        loadComponentPins(obj["visualPinList"].toArray(), c);

        componentList.append(c);
    }
}
void LibraryFile::loadComponentViews(QJsonArray views, ComponentModel *c)
{
    foreach (QJsonValue viewVal, views) {
       QJsonObject viewObject = viewVal.toObject();

       // Determine view type
       QString viewType = viewObject["viewType"].toString();

       // Parse position values: x and y
       int x = viewObject["x"].toInt();
       int y = viewObject["y"].toInt();
       // Parse main color (line or text color). Allowed formats are hex (#000000) or name ("black")
//           QString colorName = viewObject["mainColor"].toString();
//           QColor mainColor = QColor::isValidColor(colorName) ? QColor(colorName) : QColor(Qt::black);
       QColor mainColor = getColor(viewObject["mainColor"].toString());

       if(viewType == "rectangle"){
           VisualRectangle *rectangle = new VisualRectangle(x,y,mainColor);
           rectangle->width = viewObject["width"].toInt();
           rectangle->height = viewObject["height"].toInt();
           QString colorName = viewObject["fillColor"].toString();
           rectangle->fillColor = QColor::isValidColor(colorName) ? QColor(colorName) : QColor(Qt::white);
           c->visualElements.append(rectangle);
       }
       else if(viewType == "circle"){
           VisualCircle *circle = new VisualCircle(x,y,mainColor);
           circle->radius = viewObject["radius"].toInt();
           QString colorName = viewObject["fillColor"].toString();
           circle->fillColor = QColor::isValidColor(colorName) ? QColor(colorName) : QColor(Qt::white);
           c->visualElements.append(circle);
       }
       else if(viewType == "text"){
           VisualText *text = new VisualText(x,y,mainColor);
           text->text = viewObject["string"].toString();
           c->visualElements.append(text);
       }
    }
}
void LibraryFile::loadComponentPins(QJsonArray pins, ComponentModel *c)
{
    foreach (QJsonValue pinVal, pins)
    {
        QJsonObject pinObject = pinVal.toObject();

        QJsonObject pinViewObject = pinObject["view"].toObject();

        QColor mainColor = getColor(pinViewObject["lineColor"].toString());
        int x = pinViewObject["x"].toInt();
        int y = pinViewObject["y"].toInt();
        int dimension = pinViewObject["dimension"].toInt();

        PinView *pin = new PinView(mainColor, x, y, dimension, dimension);

        QString shape = pinViewObject["shape"].toString();
        pin->setShape(shape);
        pin->lineColorConnected = getColor(pinViewObject["lineColorConnected"].toString());
        pin->fillColor = getColor(pinViewObject["fillColor"].toString());
        pin->fillColorConnected = getColor(pinViewObject["fillColorConnected"].toString());
        pin->setSide(pinViewObject["side"].toString());
        c->visualPins.append(pin);

        pin->id = pinObject["ID"].toString();
        pin->title = pinObject["title"].toString();
        pin->tooltip = pinObject["tooltip"].toString();
        pin->checkConnection = pinObject["check_connection"].toString();
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

ComponentModel *LibraryFile::GetComponentByUniqueId(int uid)
{
    foreach(ComponentModel* component, this->componentList)
    {
       if(component->uid == uid)
           return component;
    }
    return 0;
}
Bus *LibraryFile::GetBusByUniqueId(int uid)
{
    foreach(RegularBus* bus, this->regularBuses)
    {
       if(bus->uid == uid)
           return bus;
    }
    return 0;
}
// Parse main color (line or text color). Allowed formats are hex (#000000) or name ("black")
QColor LibraryFile::getColor(QString colorName, bool fill)
{
    return QColor::isValidColor(colorName) ? QColor(colorName) : (fill ? QColor(Qt::white) : QColor(Qt::black));
}

