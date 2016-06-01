#include "componentview.h"
#include "visualrectangle.h"
#include "visualtext.h"
#include "Enums.h"

#include <QGraphicsView>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QPen>
#include <QPainter>
#include <QPoint>
#include <QMenu>
#include <QContextMenuEvent>

ComponentView::ComponentView(ComponentModel *model)
{
    // Set properties from model
    setId(model->id());
    setTitle(model->title());
    setInstanceNameBase(model->instanceNameBase());
    setIconFile(model->iconFile());
    setTooltip(model->tooltip());
    setMinInstances(model->minInstances());
    setMaxInstances(model->maxInstances());
    setComdelFile(model->comdelFile());
    foreach (VisualComponentElement *e, model->visualElements())
    {
        m_visualElements.append(e);
    }
    foreach (PinModel *p, model->pins())
    {
        PinView *pin = new PinView(p);
        m_pins.append(pin);
    }
    foreach (Attribute *a, model->attributes())
    {
        Attribute *attr = new Attribute(*a);
        m_attributes.append(attr);
    }

    setDimensions();

    setZValue(2);
    setFlags(ItemIsSelectable | ItemIsMovable);
}
void ComponentView::setParentToPins()
{
    foreach (PinView *pin, this->pins())
    {
        pin->setParentComponent(this);
    }
}

void ComponentView::setDimensions()
{
    int minL, minU, maxR, maxD;;
    minL = minU = std::numeric_limits<int>::max();
    maxR = maxD = std::numeric_limits<int>::min();

    foreach (VisualComponentElement *element, visualElements()) {
        VisualRectangle *rect = dynamic_cast<VisualRectangle*>(element);
        VisualText *text = dynamic_cast<VisualText*>(element);
        VisualCircle *circle = dynamic_cast<VisualCircle*>(element);

        if(rect)
        {
            qreal lineWidthCoef = (qreal)rect->lineThickness/2 + 0.5;

            if(rect->x - lineWidthCoef < minL)
                minL = rect->x - lineWidthCoef;
            if(rect->y - lineWidthCoef < minU)
                minU = rect->y - lineWidthCoef;
            if((rect->x + rect->width + lineWidthCoef) > maxR)
                maxR = rect->x + rect->width + lineWidthCoef;
            if((rect->y + rect->height + lineWidthCoef) > maxD)
                maxD = rect->y + rect->height + lineWidthCoef;
        }
        else if(text)
        {
            if(text->x < minL)
                minL = text->x;
            if(text->y < minU)
                minU = text->y;
        }
        else if(circle)
        {
            if(circle->x - circle->lineThickness/2 < minL)
                minL = circle->x - circle->lineThickness/2;
            if(circle->y - circle->lineThickness/2 < minU)
                minU = circle->y - circle->lineThickness/2;
            if(circle->x + circle->radius + circle->lineThickness/2 > maxR)
                maxR = circle->x + circle->radius + circle->lineThickness/2;
            if(circle->y + circle->radius + circle->lineThickness/2 > maxD)
                maxD = circle->y + circle->radius + circle->lineThickness/2;
        }
    }

    setX(minL);
    setY(minU);
    setWidth(maxR - minL);
    setHeight(maxD - minU);
}

QRectF ComponentView::boundingRect() const
{
    return QRectF(m_x, m_y, m_width, m_height);
}

void ComponentView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

//    QColor fillColor = (option->state & QStyle::State_Selected) ? color.dark(150) : color;
//    if (option->state & QStyle::State_MouseOver)
//         fillColor = fillColor.light(125);



    QPen oldPen = painter->pen();
    QPen pen = oldPen;
    int width = 0;
    if (option->state & QStyle::State_Selected)
     width += 2;

    pen.setWidth(width);
    QBrush b = painter->brush();

    foreach(VisualComponentElement *element, m_visualElements)
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
    pen.setWidth(rect->lineThickness);
    painter->setPen(pen);
    painter->setBrush(QBrush(rect->fillColor,Qt::SolidPattern));
    painter->drawRect(rect->x,rect->y,rect->width,rect->height);
}

void ComponentView::drawVisualText(QPainter *painter, VisualText *text)
{
    QString textDisplay;
    bool showInstanceName = text->showInstanceName;
    if(showInstanceName == true)
        textDisplay = this->instanceName();
    else
        textDisplay = text->text;
    painter->setPen(text->mainColor);
    painter->drawText(QPoint(text->x,text->y), textDisplay);
}

void ComponentView::drawVisualCircle(QPainter *painter, VisualCircle *circle)
{
    QPen pen(circle->mainColor);
    pen.setWidth(circle->lineThickness);
    painter->setPen(pen);
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

        //PinView *pin = (PinView*) pin_;
//        ds << (quint64) pin;
//        ds << pin->id;
//        ds << (int)pin->side;
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

        ds >> ptr;
        //PinView *p = (PinView*)ptr;
        //PinView *pin = new PinView();

//        ds >> pin->id;
//        int pinSide;
//        ds >> pinSide;
//        pin->side = (PinSideEnum)pin->side;

//        pinMap[ptr] = pin;
    }
}

void ComponentView::mirrorSides()
{
    setMirrored(!mirrored());
    foreach (PinView *pin, pins())
    {
        pin->switchSides();
    }
    this->update();
}

void ComponentView::setUniqueInstanceName(int num)
{
    m_instanceName = QString(instanceNameBase() + "_" + QString::number(num).rightJustified(3,'0'));
}
