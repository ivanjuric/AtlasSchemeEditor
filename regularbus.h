#include "bus.h"
#include "Enums.h"

#include <QColor>

#ifndef REGULARBUS_H
#define REGULARBUS_H

class Bus;

class RegularBus : public Bus
{
public:
    RegularBus();
    QString iconFile;
    QString title;
    QString tooltip;
    int minInstances;
    int maxInstances;

    OrientationEnum orientation;
    int lineLength;
    int lineThickness;
    QColor lineColor;

    void setOrientation(QString orientation);
    void setLineLength(int length);
    void setLineThickness(int thickness);
    void setLineColor(QColor color);
};

#endif // REGULARBUS_H
