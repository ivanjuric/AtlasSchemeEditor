#include "visualcomponentelement.h"

VisualComponentElement::VisualComponentElement(int x, int y, QColor color)
{
    this->x = x;
    this->y = y;
    this->mainColor = color;
}
QPoint VisualComponentElement::getPosition(){
    return QPoint(x,y);
}
