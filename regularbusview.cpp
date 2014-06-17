#include "regularbusview.h"
#include "regularbus.h"

#include <QGraphicsView>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QPen>
#include <QPainter>
#include <QPoint>

RegularBusView::RegularBusView(int x, int y, RegularBus *model)
{
    this->x = x;
    this->y = y;
    this->model = model;
    this->lineColor = model->lineColor;
    this->lineLength = model->lineLength;
    this->lineThickness = model->lineThickness;
    this->orientation = model->orientation;

    fillBusPins();

    setZValue(-1);

    setFlags(ItemIsSelectable | ItemIsMovable);
}

void RegularBusView::fillBusPins()
{
    double ratio = lineLength/lineThickness;
    int numberOfPins = (int)ratio;
    int remainingPinLength = lineLength - lineThickness * numberOfPins;

    if(orientation == OrientationEnum::Horizontal)
    {
        for(int i = 0; i < numberOfPins;i++)
        {
            //PinView *pin = new PinView(Qt::white, lineThickness * i, 0, lineThickness, lineThickness);
            PinModel *model = new PinModel(i);
            PinView *pin = new PinView(model);
            pin->setShape(PinTypeEnum::BusPin);
            pin->setParentItem(this);
            busPins.append(pin);
        }
    }
    else if(orientation == OrientationEnum::Vertical)
    {

    }
}

QRectF RegularBusView::boundingRect() const
{
    if(orientation == OrientationEnum:: Horizontal)
        return QRectF(x, y, lineLength, lineThickness);
    else
        return QRectF(x, y, lineThickness, lineLength);
}
void RegularBusView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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

    painter->setBrush(QBrush(fillColor, Qt::SolidPattern));
    if(orientation == OrientationEnum::Horizontal)
        painter->drawRect(0,0, lineLength,lineThickness);
    else if(orientation == OrientationEnum::Vertical)
        painter->drawRect(0,0,lineThickness, lineLength);

}
