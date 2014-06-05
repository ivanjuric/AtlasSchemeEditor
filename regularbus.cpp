#include "regularbus.h"

RegularBus::RegularBus()
{
}

void RegularBus::setOrientation(QString orientation)
{
    if(orientation == "vertical")
        this->orientation = OrientationEnum::Vertical;
    else
        this->orientation = OrientationEnum::Horizontal;
}
void RegularBus::setLineLength(int length)
{
    this->lineLength = length;
}
void RegularBus::setLineThickness(int thickness)
{
    this->lineThickness = thickness;
}

void RegularBus::setLineColor(QColor color)
{
    this->lineColor = color;
}
