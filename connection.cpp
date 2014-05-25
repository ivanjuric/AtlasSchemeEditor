#include "connection.h"
#include "pinview.h"

#include <QGraphicsItem>
#include <QBrush>
#include <QPen>
#include <QGraphicsScene>

Connection::Connection(QGraphicsItem *parent) : QGraphicsPathItem(parent)
{
    setPen(QPen(Qt::black, 2));
    setBrush(Qt::NoBrush);
    setZValue(-1);
    m_pin1 = 0;
    m_pin2 = 0;
}

Connection::~Connection()
{
    if (m_pin1)
        m_pin1->connections.remove(m_pin1->connections.indexOf(this));
    if (m_pin2)
        m_pin2->connections.remove(m_pin2->connections.indexOf(this));
}

void Connection::setPos1(const QPointF &p)
{
    pos1 = p;
}
void Connection::setPos2(const QPointF &p)
{
    pos2 = p;
}

void Connection::setPin1(PinView *p)
{
    m_pin1 = p;
    m_pin1->connections.append(this);
}
void Connection::setPin2(PinView *p)
{
    m_pin2 = p;
    m_pin2->connections.append(this);
}

void Connection::updatePosFromPins()
{
    pos1 = m_pin1->scenePos();
    pos2 = m_pin2->scenePos();
}

void Connection::updatePath()
{
    QPainterPath p;

    //QPointF pos1(m_port1->scenePos());
    //QPointF pos2(m_port2->scenePos());

    p.moveTo(pos1);

    qreal dx = pos2.x() - pos1.x();
    qreal dy = pos2.y() - pos1.y();

    QPointF ctr1(pos1.x() + dx * 0.25, pos1.y() + dy * 0.1);
    QPointF ctr2(pos1.x() + dx * 0.75, pos1.y() + dy * 0.9);

    p.cubicTo(ctr1, ctr2, pos2);

    setPath(p);
}

PinView* Connection::pin1() const
{
    return m_pin1;
}
PinView* Connection::pin2() const
{
    return m_pin2;
}
