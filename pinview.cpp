#include "pinview.h"
#include "connection.h"
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QPen>
#include <QPainter>

PinView::PinView(const QColor &lineColor, int x, int y, int dimension)
{
    this->x = x;
    this->y = y;
    this->lineColor = lineColor;
    this->dimension = dimension;
    setZValue((x + y) % 2);

    setFlags(ItemIsSelectable | ItemIsMovable );
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
}
QRectF PinView::boundingRect() const
{
    return QRectF(x, y, dimension, dimension);
}

void PinView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QColor fillColor = (option->state & QStyle::State_Selected) ? lineColor.dark(150) : lineColor;
    if (option->state & QStyle::State_MouseOver)
         fillColor = fillColor.light(125);

    QPen oldPen = painter->pen();
    QPen pen = oldPen;
    int width = 0;
    if (option->state & QStyle::State_Selected)
     width += 2;

    pen.setWidth(width);
    QBrush b = painter->brush();

    drawPin(painter,shape);

//    painter->setBrush(QBrush(fillColor.dark(option->state & QStyle::State_Sunken ? 120 : 100)));
//    painter->setPen(QPen(QColor(Qt::yellow)));
//    painter->drawRect(QRect(0, 0, 25, 25));

//    painter->setPen(QPen(QColor(Qt::white)));
//    painter->setBrush(QBrush(QColor(Qt::red),Qt::SolidPattern));
//    painter->drawText(QRect(0, 0,25, 25), Qt::AlignCenter, "Pin");
}

void PinView::setShape(QString shapeString)
{
    if(shapeString.toLower() == "in")
        shape = PinTypeEnum::In;
    else if(shapeString.toLower() == "out")
        shape = PinTypeEnum::Out;
    else if(shapeString.toLower() == "square")
        shape = PinTypeEnum::Square;
    else if(shapeString.toLower() == "circle")
        shape = PinTypeEnum::Circle;
    else if(shapeString.toLower() == "inout")
        shape = PinTypeEnum::InOut;
    else if(shapeString.toLower() == "squarein")
        shape = PinTypeEnum::SquareIn;
    else if(shapeString.toLower() == "squareout")
        shape = PinTypeEnum::SquareOut;
    else if(shapeString.toLower() == "squareinout")
        shape = PinTypeEnum::SquareInOut;
}

void PinView::drawPin(QPainter *painter, PinTypeEnum type)
{
    switch(type){
        case PinTypeEnum::Square:
            drawSquare(painter);
            break;
        case PinTypeEnum::Circle:
            drawCircle(painter);
            break;
        case PinTypeEnum::SquareInOut:
            drawSquare(painter);
            drawInOut(painter);
            break;
        case PinTypeEnum::SquareIn:
            drawSquare(painter);
            drawLR(painter);
            break;
        case PinTypeEnum::SquareOut:
            drawSquare(painter);
            drawRL(painter);
            break;
        case PinTypeEnum::In:
            drawLR(painter);
            break;
        case PinTypeEnum::Out:
            drawRL(painter);
            break;
        case PinTypeEnum::InOut:
            drawInOut(painter);
            break;
        default:
            break;
    }
}
void PinView::drawSquare(QPainter *painter)
{
    QPen pen(lineColor);
    pen.setJoinStyle(Qt::MiterJoin);
    painter->setPen(pen);
    painter->setBrush(QBrush(fillColor,Qt::SolidPattern));
    painter->drawRect(x,y,dimension,dimension);
}
void PinView::drawCircle(QPainter *painter)
{
    QPen pen(lineColor);
    pen.setJoinStyle(Qt::MiterJoin);
    painter->setPen(pen);
    painter->setBrush(QBrush(fillColor,Qt::SolidPattern));
    painter->drawEllipse(x,y,dimension,dimension);
}
void PinView::drawInOut(QPainter *painter)
{
    drawTriangle(painter,QPoint(x, y),QPoint(x + (int)dimension/2, y + (int)dimension/2),QPoint(x,y+dimension));
    drawTriangle(painter,QPoint(x+dimension, y),QPoint(x + (int)dimension/2, y + (int)dimension/2),QPoint(x+dimension,y+dimension));
}
void PinView::drawRL(QPainter *painter)
{
    drawTriangle(painter,QPoint(x+dimension, y), QPoint(x, y + (int)dimension/2), QPoint(x+dimension,y+dimension));
}
void PinView::drawLR(QPainter *painter)
{
    drawTriangle(painter,QPoint(x, y),QPoint(x + dimension, y + (int)dimension/2),QPoint(x,y+dimension));
}
void PinView::drawTriangle(QPainter *painter, QPoint a, QPoint b, QPoint c)
{
    QPolygon poly;
    poly << a << b << c;
    QPen pen(lineColor);
    //pen.setJoinStyle(Qt::MiterJoin);
    painter->setPen(pen);
    painter->setBrush(QBrush(fillColor,Qt::SolidPattern));
    painter->drawPolygon(poly);
}

QVariant PinView::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemScenePositionHasChanged)
    {
        foreach(Connection *conn, connections)
        {
            conn->updatePosFromPins();
            conn->updatePath();
        }
    }
    return value;
}

bool PinView::isConnected(PinView *other)
{
    foreach(Connection *conn, connections)
        if (conn->pin1() == other || conn->pin2() == other)
            return true;

    return false;
}
void PinView::setComponent(ComponentView *c)
{
    m_component = c;
}

