#include "visualcomponentelement.h"

#include <QString>
#include <QVector>

#ifndef COMPONENTMODEL_H
#define COMPONENTMODEL_H

class ComponentModel
{
public:
    ComponentModel();

    // Unique instance identificator
    int uid;
    // Component type unique id
    QString id;
    // Component title
    QString title;

    // Ordered list of visual elements used for component display
    QVector<VisualComponentElement*> visualElements;
};

#endif // COMPONENTMODEL_H
