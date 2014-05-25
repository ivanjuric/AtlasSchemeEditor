#include <QGraphicsItem>

#ifndef PINVIEW_H
#define PINVIEW_H

class Connection;
class ComponentView;

class PinView : public QGraphicsItem
{
public:
    PinView(const QColor &lineColor, int x, int y, int dimension);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);
    enum PinTypeEnum
      {
         Square,Circle,In,Out,InOut,SquareIn,SquareOut,SquareInOut
      };
    int uid;
    QString id;
    QString title;
    QString tooltip;
    QString checkConnection;
    PinTypeEnum shape;
    int x;
    int y;
    int dimension;
    QColor lineColor;
    QColor fillColor;
    QColor lineColorConnected;
    QColor fillColorConnected;
    void setShape(QString shapeString);

    void drawPin(QPainter *painter, PinTypeEnum type);
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
    ComponentView* component() const { return m_component; }
    bool isConnected(PinView*);
    void setComponent(ComponentView *c);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    ComponentView *m_component;
};

#endif // PINVIEW_H
