#include "pinview.h"
#include "connection.h"
#include "componentview.h"
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QPen>
#include <QPainter>


PinView::PinView(PinModel *model)
{
    setUid(model->uid());
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

    label = new QGraphicsTextItem(this);
    margin = 2;
    setZValue((x() + y()) % 2);
    setFlags(ItemIsMovable );
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
}
void PinView::setParentComponent(ComponentView *parentComponent)
{
    m_parentComponent = parentComponent;
    setParentInstanceName(parentComponent->instanceName());
}

QRectF PinView::boundingRect() const
{
    QRect r(m_x, m_y, m_width, m_height);
    return QRectF(r);
}

void PinView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QColor fillColor = (option->state & QStyle::State_Selected) ? lineColor().dark(150) : lineColor();
    if (option->state & QStyle::State_MouseOver)
         fillColor = fillColor.light(125);

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
void PinView::drawBusPin(QPainter *painter)
{
    QPen pen(Qt::yellow);
    painter->setPen(pen);
    painter->setBrush(Qt::transparent);
    painter->drawRect(x(),y(),width(), height());
}
void PinView::drawSquare(QPainter *painter)
{
    QPen pen(lineColor());
    pen.setJoinStyle(Qt::MiterJoin);
    painter->setPen(pen);
    painter->setBrush(QBrush(fillColor(),Qt::SolidPattern));
    painter->drawRect(x(),y(),width(), height());
}
void PinView::drawCircle(QPainter *painter)
{
    QPen pen(lineColor());
    pen.setJoinStyle(Qt::MiterJoin);
    painter->setPen(pen);
    painter->setBrush(QBrush(fillColor(),Qt::SolidPattern));
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
    //pen.setJoinStyle(Qt::MiterJoin);
    painter->setPen(pen);
    painter->setBrush(QBrush(fillColor(),Qt::SolidPattern));
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
        point->setX(this->parentItem()->pos().x() + x());
        point->setY(this->parentItem()->pos().y() + y());
    }

    return point;
}
void PinView::setStartPosition()
{
    QPointF *point = getStartPosition();
    setX(point->x());
    setY(point->y());
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
    label->setPlainText(this->id().toUpper());
    label->setFont(QFont("Arial", 5));

    QRectF r = label->boundingRect();

    QPointF *point = this->getStartPosition();

    if (side() == PinSideEnum::Left)
    {
        point->setX(point->x() + width());
        point->setY(point->y() + height()/2 - r.height()/2);
        label->setPos(*point);
    }
    else if(side() == PinSideEnum::Right)
    {
        point->setX(point->x() - r.right());
        point->setY(point->y() + height()/2 - r.height()/2);
        label->setPos(*point);
    }
}

void PinView::switchSides()
{
    if(side() == PinSideEnum::Left)
        setSide(PinSideEnum::Right);
    else if(side() == PinSideEnum::Right)
        setSide(PinSideEnum::Left);

    updatePositionAfterMirror();
    this->update();
}

void PinView::updatePositionAfterMirror()
{
    if(side() == PinSideEnum::Left)
        setX(x() - parentComponent()->width() - width());
    else if(side() == PinSideEnum::Right)
        setX(x() + parentComponent()->width() + width());
}


