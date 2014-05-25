#include "pinview.h"
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
    QString id;
    QString title;
    QString instanceNameBase;
    QString iconFile;
    QString tooltip;
    int minInstances;
    int maxInstances;
    QString comdelFile;

    // Ordered list of visual elements used for component display
    QVector<VisualComponentElement*> visualElements;

    // List of visual pins
    QVector<PinView*> visualPins;
};

#endif // COMPONENTMODEL_H
