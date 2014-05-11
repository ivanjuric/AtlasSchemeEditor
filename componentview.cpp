#include "componentview.h"
#include "visualrectangle.h"
#include "visualtext.h"

#include <QGraphicsView>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QPen>
#include <QPainter>
#include <QPoint>

ComponentView::ComponentView(int x, int y, ComponentModel *model)
{
    this->x = x;
    this->y = y;
    this->model = model;
    setZValue((x + y) % 2);

    setFlags(ItemIsSelectable | ItemIsMovable);
}

QRectF ComponentView::boundingRect() const
{
    return QRectF(0, 0, 100, 100);
}

QPainterPath ComponentView::shape() const
{
    QPainterPath path;
    path.addRect(14, 14, 100, 100);
    return path;
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
    painter->setPen(rect->mainColor);
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
    //painter->setBrush(QBrush(circle->fillColor, Qt::SolidPattern));
    painter->drawEllipse(circle->x,circle->y,circle->radius, circle->radius);
}
