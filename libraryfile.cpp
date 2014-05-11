#include "libraryfile.h"
#include "memory.h"
#include "visualrectangle.h"
#include "visualtext.h"
#include "visualcircle.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QFile>
#include <qjsonarray.h>

// Represents initial library file that contains all informations
// needed to prepare main screen and whole system to work with
// specified computer system.
LibraryFile::LibraryFile(QString filepath){
    loadJson(filepath);
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

    QJsonObject jsonObject = doc.object();

    libraryName = jsonObject["libraryName"].toString();
    libraryInfo = jsonObject["libraryInfo"].toString();

    QJsonArray components = jsonObject["componentList"].toArray();

    // Generate random id for components
    int randId = rand();
    foreach (QJsonValue val, components){
        QJsonObject obj = val.toObject();

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
        c->id = id;
        c->title = title;

        // Load component views
        QJsonArray views = obj["view"].toArray();

        foreach (QJsonValue viewVal, views) {
           QJsonObject viewObject = viewVal.toObject();

           // Determine view type
           QString viewType = viewObject["viewType"].toString();

           // Parse position values: x and y
           int x = viewObject["x"].toInt();
           int y = viewObject["y"].toInt();
           // Parse main color (line or text color). Allowed formats are hex (#000000) or name ("black")
           QString colorName = viewObject["mainColor"].toString();
           QColor mainColor = QColor::isValidColor(colorName) ? QColor(colorName) : QColor(Qt::black);

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
               c->visualElements.append(circle);
           }
           else if(viewType == "text"){
               VisualText *text = new VisualText(x,y,mainColor);
               text->text = viewObject["string"].toString();
               c->visualElements.append(text);
           }
        }

        componentList.append(c);
    }

    return true;
}

ComponentModel *LibraryFile::GetComponentByUniqueId(int uid){
    foreach(ComponentModel* component, this->componentList){
       if(component->uid == uid)
           return component;
    }
    return 0;
}

