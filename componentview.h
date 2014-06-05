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
    ComponentView();
    ComponentView(int x, int y, ComponentModel *model);

    int x, y, width, height;
    QColor color;

    ComponentModel *model;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);
    enum { Type = QGraphicsItem::UserType + 3 };
    int type() const { return Type; }

    void save(QDataStream &ds);
    void load(QDataStream &ds, QMap<quint64, PinView*> &pinMap);

private:


     //void drawShapeToComponent(QPainter painter);
     void drawVisualElement(QPainter *painter, VisualComponentElement *visualElement);
     void drawVisualRectangle(QPainter *painter, VisualRectangle *rect);
     void drawVisualText(QPainter *painter, VisualText *text);
     void drawVisualCircle(QPainter *painter, VisualCircle *circle);
     void setDimensions();
};

#endif // COMPONENTVIEW_H
