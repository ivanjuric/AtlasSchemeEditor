#include "visualrectangle.h"

VisualRectangle::VisualRectangle(int x, int y, QColor color)
    : VisualComponentElement(x, y, color)
{
}
VisualRectangle::VisualRectangle(VisualRectangle *rect)
: VisualComponentElement(rect->x, rect->y, rect->mainColor)
{
    this->fillColor = rect->fillColor;
    this->lineThickness = rect->lineThickness;
    this->width = rect->width;
    this->height = rect->height;
}
