#include "regularbus.h"
#include "Enums.h"
#include "pinview.h"
#include <QGraphicsItem>

#ifndef BUSVIEW_H
#define BUSVIEW_H

class RegularBus;

class BusView : public QGraphicsItem
{
public:
    BusView(int x, int y, RegularBus *model);

    OrientationEnum orientation;
    int lineLength;
    int lineThickness;
    QColor lineColor;
    int x, y;

    QVector<PinView*> busPins;

    RegularBus *model;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);

    void setOrientation(QString orientation);
    void setLineLength(int length);
    void setLineThickness(int thickness);
    void setLineColor(QColor color);

private:
    void fillBusPins();
};
#endif // BUSVIEW_H
