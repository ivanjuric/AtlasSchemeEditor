#include "libraryfile.h"
#include "memory.h"
#include "processor.h"
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

    foreach (QJsonValue val, components){
        QJsonObject obj = val.toObject();
        QString name = obj["name"].toString();
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

        BaseComponent *cpu = new Processor();
        BaseComponent *ram = new Memory();


        componentList.append(cpu);
        componentList.append(ram);
    }

    return true;
}
