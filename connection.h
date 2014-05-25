#include <QGraphicsPathItem>

#ifndef CONNECTION_H
#define CONNECTION_H

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

    void updatePosFromPins();
    void updatePath();
    PinView* pin1() const;
    PinView* pin2() const;
private:
    QPointF pos1;
    QPointF pos2;
    PinView *m_pin1;
    PinView *m_pin2;
};

#endif // CONNECTION_H
