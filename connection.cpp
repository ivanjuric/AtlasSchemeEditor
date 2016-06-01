#include "connection.h"
#include "pinview.h"

#include "qgraphicsitem.h"
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
    m_automaticBus = 0;
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
    m_pos1 = p;
}
void Connection::setPos2(const QPointF &p)
{
    m_pos2 = p;
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
    m_pos1 = m_pin1->scenePos();
    m_pos2 = m_pin2->scenePos();
}

void Connection::updatePath()
{
    QPainterPath p;

    if(pin1())
        m_pos1 = pin1()->centerPos(pin1());
    if(pin2())
        m_pos2 = pin2()->centerPos(pin2());

    p.moveTo(m_pos1);

    qreal dx = m_pos2.x() - m_pos1.x();
    qreal dy = m_pos2.y() - m_pos1.y();

    QPointF ctr1(m_pos1.x() + dx * 0.25, m_pos1.y() + dy * 0.1);
    QPointF ctr2(m_pos1.x() + dx * 0.75, m_pos1.y() + dy * 0.9);

    p.cubicTo(ctr1, ctr2, m_pos2);

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

void Connection::setAutomaticBus(AutomaticBus *bus)
{
    AutomaticBus *b = new AutomaticBus(*bus);
    m_automaticBus = b;
}
