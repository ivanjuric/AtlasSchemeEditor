#include <QGraphicsItem>
#include "Enums.h"

#ifndef PINVIEW_H
#define PINVIEW_H

class Connection;
class ComponentView;

class PinView : public QGraphicsItem
{
public:
    PinView(const QColor &lineColor, int x, int y, int width, int height);
    PinView();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);

    int uid;
    QString id;
    QString title;
    QString tooltip;
    QString checkConnection;
    PinTypeEnum shape;
    int x;
    int y;
    int width;
    int height;
    enum PinSideEnum {Left, Right};
    PinSideEnum side;
    QColor lineColor;
    QColor fillColor;
    QColor lineColorConnected;
    QColor fillColorConnected;
    void setShape(QString shapeString);

    void drawPin(QPainter *painter, PinTypeEnum type);
    void drawBusPin(QPainter *painter);
    void drawSquare(QPainter *painter);
    void drawCircle(QPainter *painter);
    void drawInOut(QPainter *painter);
    void drawRL(QPainter *painter);
    void drawLR(QPainter *painter);
    void drawTriangle(QPainter *painter, QPoint a, QPoint b, QPoint c);

    QVector<Connection*> connections;

    enum { Type = QGraphicsItem::UserType + 1 };
    enum { NamePort = 1, TypePort = 2 };
    int type() const { return Type; }
    ComponentView* component() { return m_component; }
    bool isConnected(PinView*);
    void setComponent(ComponentView *c);
    void setSide(QString side);

    QPointF *getStartPosition();
    void setStartPosition();
    QPointF centerPos(PinView *pin);

    QGraphicsTextItem *label;
    void setLabel();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    ComponentView *m_component;
    int margin;
};

#endif // PINVIEW_H
