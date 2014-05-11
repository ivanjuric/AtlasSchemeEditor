#include <QString>
#include <QVector>
#include "componentview.h"
#include "componentmodel.h"

#ifndef LIBRARYFILE_H
#define LIBRARYFILE_H

class LibraryFile
{
public:
    LibraryFile(QString filepath);
    ComponentModel* GetComponentByUniqueId(int id);

    // Properties
    QString libraryName;
    QString libraryInfo;
    QVector<ComponentModel*> componentList;

private:
    bool loadJson(QString filepath);
};

#endif // LIBRARYFILE_H
