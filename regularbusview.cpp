#include "regularbusview.h"
#include "regularbus.h"

#include <QGraphicsView>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QPen>
#include <QPainter>
#include <QPoint>

RegularBusView::RegularBusView(RegularBus *model, QPoint pos)
{
    setX(pos.x());
    setY(pos.y());
    setIconFile(model->iconFile());
    setTitle(model->title());
    setTooltip(model->tooltip());
    setMinInstances(model->minInstances());
    setMaxInstances(model->maxInstances());
    setLineColor(model->lineColor());
    setLineLength(model->lineLength());
    setLineThickness(model->lineThickness());
    setOrientation(model->orientation());

    //setUid(model->uid());
    setId(model->id());
    foreach (BusLine *busLine, model->busLines())
    {
        addBusLine(busLine);
    }
    m_label = new QGraphicsTextItem(this);
    fillBusPins();
    setZValue(-1);
    setFlags(ItemIsSelectable | ItemIsMovable);
}

void RegularBusView::fillBusPins()
{
    double ratio = lineLength()/lineThickness();
    int numberOfPins = (int)ratio;
    int remainingPinLength = lineLength() - lineThickness() * numberOfPins;
    QColor color = Qt::yellow;
    int a = lineThickness();

    if(orientation() == OrientationEnum::Horizontal)
    {
        for(int i = 0; i < numberOfPins;i++)
        {
            PinView *pin = createPinForBus(i, color, a * i, 0, a, a);
            addBusPin(pin);
        }
        if(remainingPinLength > 0)
        {
            PinView *pin = createPinForBus(busPins().length(), color, lineLength() - remainingPinLength, 0, remainingPinLength, a);
            addBusPin(pin);
        }
    }
    else if(orientation() == OrientationEnum::Vertical)
    {
        for(int i = 0; i < numberOfPins;i++)
        {
            PinView *pin = createPinForBus(i, color, 0, a * i, a, a);
            addBusPin(pin);
        }
        if(remainingPinLength > 0)
        {
            PinView *pin = createPinForBus(busPins().length(),color, 0, lineLength() - remainingPinLength, a, remainingPinLength);
            addBusPin(pin);
        }
    }
}
PinView *RegularBusView::createPinForBus(int uid, QColor color, int x, int y, int width, int height)
{
    PinModel *model = new PinModel(uid);
    model->setLineColor(color);
    model->setX(x);
    model->setY(y);
    model->setWidth(width);
    model->setHeight(height);
    PinView *pin = new PinView(model);
    pin->setLineColor(color);
    pin->setShape(PinTypeEnum::BusPin);
    pin->setParentItem(this);
    pin->setParentBus(this);
    pin->setSide(PinSideEnum::None);

    return pin;
}

QRectF RegularBusView::boundingRect() const
{
    if(m_orientation == OrientationEnum:: Horizontal)
        return QRectF(m_x, m_y, m_lineLength, m_lineThickness);
    else
        return QRectF(m_x, m_y, m_lineThickness, m_lineLength);
}
void RegularBusView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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

    painter->setBrush(QBrush(fillColor, Qt::SolidPattern));
    if(m_orientation == OrientationEnum::Horizontal)
        painter->drawRect(0,0, m_lineLength,m_lineThickness);
    else if(m_orientation == OrientationEnum::Vertical)
        painter->drawRect(0,0,m_lineThickness, m_lineLength);

}

void RegularBusView::addBusPin(PinView *pin)
{
    m_busPins.append(pin);
}
void RegularBusView::setUniqueInstanceName(int num)
{
    m_instanceName = QString(id() + "_" + QString::number(num).rightJustified(3,'0'));
}
void RegularBusView::addBusLine(BusLine *busLine)
{
    m_busLines.append(busLine);
}
void RegularBusView::setLabel()
{
    m_label->setPlainText(title().toUpper());
    m_label->setFont(QFont("Arial", 8));

    updateLabelPosition();
}
void RegularBusView::updateLabelPosition()
{
    QPointF point(x(),y());
    QRectF r = m_label->boundingRect();
    if (orientation() == OrientationEnum::Horizontal)
    {
        point.setX(point.x() + lineLength());
        point.setY(point.y() + lineThickness()/2 - r.height()/2);
        m_label->setPos(point);
    }
    else if(orientation() == OrientationEnum::Vertical)
    {
        point.setX(point.x() + lineThickness());
        point.setY(point.y() + lineLength() - r.height());
        m_label->setPos(point);
    }
}
