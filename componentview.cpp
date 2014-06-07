#include "componentview.h"
#include "visualrectangle.h"
#include "visualtext.h"

#include <QGraphicsView>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QPen>
#include <QPainter>
#include <QPoint>

ComponentView::ComponentView()
{
}

ComponentView::ComponentView(int x, int y, ComponentModel *model)
{
    this->x = x;
    this->y = y;
    this->model = model;
    setZValue((x + y) % 2);

    setFlags(ItemIsSelectable | ItemIsMovable);

    //set initial width and height
    width = 100;
    height = 100;

    foreach(PinView* pin, model->visualPins)
    {
        pin->setComponent(this);
        pin->setLabel();
    }
}

void ComponentView::setDimensions()
{
    int minL, minU, maxR, maxD;;
    minL = minU = std::numeric_limits<int>::max();
    maxR = maxD = std::numeric_limits<int>::min();

    foreach (VisualComponentElement *element, this->model->visualElements) {
        VisualRectangle *rect = dynamic_cast<VisualRectangle*>(element);
        VisualText *text = dynamic_cast<VisualText*>(element);
        VisualCircle *circle = dynamic_cast<VisualCircle*>(element);

        if(rect){
            if(rect->x < minL)
                minL = rect->x;
            if(rect->y < minU)
                minU = rect->y;
            if((rect->x + rect->width) > maxR)
                maxR = rect->x + rect->width;
            if((rect->y + rect->width) > maxD)
                maxD = rect->y + rect->width;
        }
        else if(text){
            if(text->x < minL)
                minL = text->x;
            if(text->y < minU)
                minU = text->y;
        }
        else if(circle){

        }
    }

    x = minL;
    y = maxD;
    width = maxR - minL;
    height = maxD - minU;
}

QRectF ComponentView::boundingRect() const
{
    return QRectF(x, y, width, height);
}

void ComponentView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QColor fillColor = (option->state & QStyle::State_Selected) ? color.dark(150) : color;
    if (option->state & QStyle::State_MouseOver)
         fillColor = fillColor.light(125);



    QPen oldPen = painter->pen();
    QPen pen = oldPen;
    int width = 0;
    if (option->state & QStyle::State_Selected)
     width += 2;

    pen.setWidth(width);
    QBrush b = painter->brush();

//    painter->setBrush(QBrush(fillColor.dark(option->state & QStyle::State_Sunken ? 120 : 100)));
//    painter->setPen(QPen(QColor(Qt::yellow)));
//    painter->drawRect(QRect(0, 0, 100, 100));

//    painter->setPen(QPen(QColor(Qt::black)));
//    painter->setBrush(QBrush(QColor(Qt::red),Qt::SolidPattern));
//    painter->drawRect(QRect(10, 10, 70, 70));

//    painter->setPen(QPen(QColor(Qt::white)));
//    painter->setBrush(QBrush(QColor(Qt::red),Qt::SolidPattern));
//    painter->drawText(QRect(10, 10, 70, 70), Qt::AlignCenter, "Qt");



    foreach(VisualComponentElement *element, model->visualElements)
    {
        drawVisualElement(painter,element);
    }
}


void ComponentView::drawVisualElement(QPainter *painter, VisualComponentElement *visualElement)
{
    VisualRectangle *rect = dynamic_cast<VisualRectangle*>(visualElement);
    VisualText *text = dynamic_cast<VisualText*>(visualElement);
    VisualCircle *circle = dynamic_cast<VisualCircle*>(visualElement);
    if(rect)
        drawVisualRectangle(painter,rect);
    else if(circle)
        drawVisualCircle(painter,circle);
    else if(text)
        drawVisualText(painter,text);


}

void ComponentView::drawVisualRectangle(QPainter *painter, VisualRectangle *rect)
{
    QPen pen(rect->mainColor);
    pen.setJoinStyle(Qt::MiterJoin);
    painter->setPen(pen);
    painter->setBrush(QBrush(rect->fillColor,Qt::SolidPattern));
    painter->drawRect(rect->x,rect->y,rect->width,rect->height);
}

void ComponentView::drawVisualText(QPainter *painter, VisualText *text)
{
    painter->setPen(text->mainColor);
    painter->drawText(QPoint(text->x,text->y), text->text);
}

void ComponentView::drawVisualCircle(QPainter *painter, VisualCircle *circle)
{
    painter->setPen(circle->mainColor);
    painter->setBrush(QBrush(circle->fillColor, Qt::SolidPattern));
    painter->drawEllipse(circle->x,circle->y,circle->radius, circle->radius);
}

void ComponentView::save(QDataStream &ds)
{
    ds << pos();

    int count(0);

    foreach(QGraphicsItem *pin, childItems())
    {
        if (pin->type() != PinView::Type)
            continue;
        count++;
    }

    ds << count;

    foreach(QGraphicsItem *pin_, childItems())
    {
        if (pin_->type() != PinView::Type)
            continue;

        PinView *pin = (PinView*) pin_;
        ds << (quint64) pin;
        ds << pin->id;
        ds << (int)pin->side;
    }
}

void ComponentView::load(QDataStream &ds, QMap<quint64, PinView*> &pinMap)
{
    QPointF p;
    ds >> p;
    setPos(p);
    int count;
    ds >> count;
    for (int i = 0; i < count; i++)
    {
        quint64 ptr;

        ds >> (quint64) ptr;
        PinView *p = (PinView*)ptr;
        PinView *pin = new PinView();

        ds >> pin->id;
        int pinSide;
        ds >> pinSide;
        pin->side = (PinView::PinSideEnum)pinSide;

        pinMap[ptr] = pin;
    }
}
