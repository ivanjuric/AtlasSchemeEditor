#include "visualcomponentelement.h"

VisualComponentElement::VisualComponentElement(int x, int y, QColor color)
{
    this->x = x;
    this->y = y;
    this->mainColor = color;
}
VisualComponentElement::VisualComponentElement(VisualComponentElement *element)
{
    this->x = element->x;
    this->y = element->y;
    this->mainColor = element->mainColor;
}
