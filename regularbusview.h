#include "regularbus.h"
#include "Enums.h"
#include "pinview.h"
#include <QGraphicsItem>

#ifndef REGULARBUSVIEW_H
#define REGULARBUSVIEW_H

class RegularBusView : public QGraphicsItem
{
public:
    RegularBusView(int x, int y, RegularBus *model);

    OrientationEnum orientation;
    int lineLength;
    int lineThickness;
    QColor lineColor;
    int x, y;

    QVector<PinView*> busPins;

    RegularBus *model;

    enum { Type = QGraphicsItem::UserType + 5 };

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);

    void setOrientation(QString orientation);
    void setLineLength(int length);
    void setLineThickness(int thickness);
    void setLineColor(QColor color);

private:
    void fillBusPins();
};
#endif // REGULARBUSVIEW_H
