#include "automaticbus.h"

#include <QGraphicsPathItem>

#ifndef CONNECTION_H
#define CONNECTION_H

#pragma once

class PinView;

class Connection : public QGraphicsPathItem
{
public:
    Connection(QGraphicsItem *parent = 0);
    ~Connection();
    void setPos1(const QPointF &p);
    void setPos2(const QPointF &p);
    void setPin1(PinView *p);
    void setPin2(PinView *p);
    void setAutomaticBus(AutomaticBus *bus);

    void updatePosFromPins();
    void updatePath();
    PinView* pin1() const;
    PinView* pin2() const;
    AutomaticBus* automaticBus() { return m_automaticBus; }
private:
    QPointF pos1;
    QPointF pos2;
    PinView *m_pin1;
    PinView *m_pin2;
    AutomaticBus *m_automaticBus;
};

#endif // CONNECTION_H
