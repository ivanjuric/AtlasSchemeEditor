#include <QString>
#include <QVector>
#include "componentview.h"
#include "componentmodel.h"
#include "addressspace.h"
#include "messages.h"
#include "regularbus.h"
#include "automaticbus.h"
#include "busview.h"

#ifndef LIBRARYFILE_H
#define LIBRARYFILE_H

class LibraryFile
{
public:
    LibraryFile(QString filepath);
    ComponentModel* GetComponentByUniqueId(int id);
    Bus *GetBusByUniqueId(int uid);


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
    Messages messages;

private:
    int randId;

    bool loadJson(QString filepath);
    void loadComdelHeader(QJsonArray headerLines);
    void loadAddressSpaceList(QJsonArray addressSpaces);
    void loadMessages(QJsonObject messagesObj);
    void loadComponents(QJsonArray compArray);
    void loadComponentViews(QJsonArray views, ComponentModel *c);
    void loadComponentPins(QJsonArray pins, ComponentModel *c);
    void loadBuses(QJsonArray busList, bool regularBusType = false);
    void loadBusLines(QJsonArray busLines, Bus *bus);
    void loadBusView(QJsonObject view, RegularBus *bus);
    QColor getColor(QString colorName, bool fill = false);
};

#endif // LIBRARYFILE_H