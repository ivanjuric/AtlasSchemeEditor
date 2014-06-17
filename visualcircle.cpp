#include "visualcircle.h"

VisualCircle::VisualCircle(int x, int y, QColor color)
    : VisualComponentElement(x, y, color)
{
}
VisualCircle::VisualCircle(VisualCircle *circle)
: VisualComponentElement(circle->x, circle->y, circle->mainColor)
{
    this->fillColor = circle->fillColor;
    this->lineThickness = circle->lineThickness;
    this->radius = circle->radius;
}
