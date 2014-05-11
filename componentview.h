#include "componentmodel.h"
#include "visualcircle.h"
#include "visualrectangle.h"
#include "visualtext.h"

#include <QGraphicsItem>

#ifndef COMPONENTVIEW_H
#define COMPONENTVIEW_H

class ComponentView : public QGraphicsItem
{
public:
    ComponentView(int x, int y, ComponentModel *model);

    ComponentModel *model;

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);

private:
     int x, y;
     QColor color;
     //void drawShapeToComponent(QPainter painter);
     void drawVisualElement(QPainter *painter, VisualComponentElement *visualElement);
     void drawVisualRectangle(QPainter *painter, VisualRectangle *rect);
     void drawVisualText(QPainter *painter, VisualText *text);
     void drawVisualCircle(QPainter *painter, VisualCircle *circle);
};

#endif // COMPONENTVIEW_H
