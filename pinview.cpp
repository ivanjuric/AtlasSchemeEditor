#include "pinview.h"
#include "connection.h"
#include "componentview.h"
#include "regularbusview.h"
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QPen>
#include <QPainter>

PinView::PinView()
{
}

PinView::PinView(PinModel *model)
{
    setId(model->id());
    setTitle(model->title());
    setTooltip(model->tooltip());
    setShape(model->shape());
    setX(model->x());
    setY(model->y());
    setWidth(model->width());
    setHeight(model->height());
    setSide(model->side());
    setLineColor(model->lineColor());
    setFillColor(model->fillColor());
    setLineColorConnected(model->lineColorConnected());
    setFillColorConnected(model->fillColorConnected());
    m_parentBus = 0;
    m_parentComponent = 0;

    label = new QGraphicsTextItem(this);
    margin = 2;
    setZValue((x() + y()) % 2);
    setFlags(ItemIsMovable );
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
    setAcceptHoverEvents(true);
}
void PinView::setParentComponent(ComponentView *parentComponent)
{
    m_parentComponent = parentComponent;
    setParentInstanceName(parentComponent->instanceName());
}
void PinView::setParentBus(RegularBusView *parentBus)
{
    m_parentBus = parentBus;
    setParentInstanceName(parentBus->instanceName());
}
QRectF PinView::boundingRect() const
{
    QRect r(m_x, m_y, m_width, m_height);
    return QRectF(r);
}

void PinView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QPen oldPen = painter->pen();
    QPen pen = oldPen;
    int width = 0;
    if (option->state & QStyle::State_Selected)
     width += 2;

    pen.setWidth(width);
    QBrush b = painter->brush();

    drawPin(painter,shape());
}

void PinView::drawPin(QPainter *painter, PinTypeEnum type)
{
    switch(type){
        case PinTypeEnum::BusPin:
            drawBusPin(painter);
            break;
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
            if(side() == PinSideEnum::Left)
                drawLR(painter);
            else
                drawRL(painter);
            break;
        case PinTypeEnum::SquareOut:
            drawSquare(painter);
            if(side() == PinSideEnum::Left)
                drawRL(painter);
            else
                drawLR(painter);
            break;
        case PinTypeEnum::In:
            if(side() == PinSideEnum::Left)
                drawLR(painter);
            else
                drawRL(painter);
            break;
        case PinTypeEnum::Out:
            if(side() == PinSideEnum::Left)
                drawRL(painter);
            else
                drawLR(painter);
            break;
        case PinTypeEnum::InOut:
            drawInOut(painter);
            break;
        default:
            break;
    }
}
void PinView::drawBusPin(QPainter *painter)
{
    QPen pen(lineColor());
    painter->setPen(pen);
    painter->setBrush(Qt::transparent);
    painter->drawRect(x(),y(),width(), height());
}
void PinView::drawSquare(QPainter *painter)
{
    QPen pen(lineColor());
    pen.setJoinStyle(Qt::MiterJoin);
    QBrush brush(fillColor(),Qt::SolidPattern);
    if(this->isConnected())
    {
        pen.setColor(lineColorConnected());
        brush.setColor(fillColorConnected());
    }
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawRect(x(),y(),width(), height());
}
void PinView::drawCircle(QPainter *painter)
{
    QPen pen(lineColor());
    pen.setJoinStyle(Qt::MiterJoin);
    QBrush brush(fillColor(),Qt::SolidPattern);
    if(this->isConnected())
    {
        pen.setColor(lineColorConnected());
        brush.setColor(fillColorConnected());
    }
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawEllipse(x(),y(),width(), height());
}
void PinView::drawInOut(QPainter *painter)
{
    drawTriangle(painter,QPoint(x(), y()),QPoint(x() + (int)width()/2, y() + (int)height()/2),QPoint(x(),y()+height()));
    drawTriangle(painter,QPoint(x()+width(), y()),QPoint(x() + (int)height()/2, y() + (int)height()/2),QPoint(x()+width(),y()+height()));
}
void PinView::drawRL(QPainter *painter)
{
    drawTriangle(painter,QPoint(x()+width(), y()), QPoint(x(), y() + (int)height()/2), QPoint(x()+width(),y()+height()));
}
void PinView::drawLR(QPainter *painter)
{
    drawTriangle(painter,QPoint(x(), y()),QPoint(x() + width(), y() + (int)height()/2),QPoint(x(),y()+height()));
}
void PinView::drawTriangle(QPainter *painter, QPoint a, QPoint b, QPoint c)
{
    QPolygon poly;
    poly << a << b << c;
    QPen pen(lineColor());
    QBrush brush(fillColor(),Qt::SolidPattern);
    if(this->isConnected())
    {
        pen.setColor(lineColorConnected());
        brush.setColor(fillColorConnected());
    }
    painter->setPen(pen);
    painter->setBrush(brush);
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

bool PinView::isConnected()
{
    foreach(Connection *conn, connections)
    {
        PinView *p1 = conn->pin1();
        PinView *p2 = conn->pin2();
        if((p1 && p1->parentInstanceName() == this->parentInstanceName() && p1->id() == this->id() && p2 != 0)
          || (p2 && p2->parentInstanceName() == this->parentInstanceName() && p2->id() == this->id() && p2 != 0))
            return true;
    }

    return false;
}

QPointF* PinView::getStartPosition()
{
    QPointF *point = new QPointF();
    if(side() == PinSideEnum::Left)
    {
        point->setX(parentComponent()->x() - x() - width());
        point->setY(parentComponent()->y() + y());
    }
    else if(side() == PinSideEnum::Right)
    {
        point->setX(parentComponent()->x() + parentComponent()->width() + x());
        point->setY(parentComponent()->y() + y());
    }
    else
    {
        QPoint p = this->parentBus()->pos().toPoint();
        point->setX(p.x() + x());
        point->setY(p.y() + y());
    }

    return point;
}
void PinView::setStartPosition()
{
    QPoint point = getStartPosition()->toPoint();
    setX(point.x());
    setY(point.y());
}

QPointF PinView::centerPos(PinView *pin)
{
    QPointF point = pin->parentItem()->scenePos();
    point.setX(point.x() + x() + pin->width()/2);
    point.setY(point.y() + y() + pin->height()/2);
    return point;
}

void PinView::setLabel()
{
    if(title() != "")
        label->setPlainText(this->title().toUpper());
    label->setFont(QFont("Arial", 5));

    updateLabelPosition();
}

void PinView::switchSides()
{
    if(side() == PinSideEnum::Left)
        setSide(PinSideEnum::Right);
    else if(side() == PinSideEnum::Right)
        setSide(PinSideEnum::Left);

    updatePositionAfterMirror();
    updateLabelPosition();
    this->update();
}

void PinView::updatePositionAfterMirror()
{
    if(side() == PinSideEnum::Left)
    {
        setX(x() - parentComponent()->width() - width());

    }
    else if(side() == PinSideEnum::Right)
    {
        setX(x() + parentComponent()->width() + width());
    }
}

void PinView::updateLabelPosition()
{
    QPointF point(x(),y());
    QRectF r = label->boundingRect();
    if (side() == PinSideEnum::Left)
    {
        point.setX(point.x() + width());
        point.setY(point.y() + height()/2 - r.height()/2);
        label->setPos(point);
    }
    else if(side() == PinSideEnum::Right)
    {
        point.setX(point.x() - r.right());
        point.setY(point.y() + height()/2 - r.height()/2);
        label->setPos(point);
    }
}

void PinView::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::setToolTip(tooltip());
}
