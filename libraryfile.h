#include <QString>
#include <QVector>
#include "componentview.h"
#include "componentmodel.h"
#include "addressspace.h"
#include "messages.h"

#ifndef LIBRARYFILE_H
#define LIBRARYFILE_H

class LibraryFile
{
public:
    LibraryFile(QString filepath);
    ComponentModel* GetComponentByUniqueId(int id);

    // Properties
    QString libraryTitle;
    QString libraryInfo;
    QVector<QString> comdelHeader;
    QString comdelDirectory;
    QVector<AddressSpace*> addressSpaceList;
    QVector<ComponentModel*> componentList;
    Messages messages;

private:
    int randId;

    bool loadJson(QString filepath);
    void loadComdelHeader(QJsonArray headerLines);
    void loadAddressSpaceList(QJsonArray addressSpaces);
    void loadMessages(QJsonObject messagesObj);
    void loadComponents(QJsonArray compArray);
    QColor getColor(QString colorName, bool fill = false);
};

#endif // LIBRARYFILE_H
