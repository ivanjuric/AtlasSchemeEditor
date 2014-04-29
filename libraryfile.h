#include <QString>
#include <QVector>
#include "basecomponent.h"

#ifndef LIBRARYFILE_H
#define LIBRARYFILE_H

class LibraryFile
{
public:
    LibraryFile(QString filepath);

    // Properties
    QString libraryName;
    QString libraryInfo;
    QVector<BaseComponent*> componentList;

private:
    bool loadJson(QString filepath);
};

#endif // LIBRARYFILE_H
