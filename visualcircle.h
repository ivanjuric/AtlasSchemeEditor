#include "visualcomponentelement.h"

#ifndef VISUALCIRCLE_H
#define VISUALCIRCLE_H

#pragma once


class VisualCircle  : public VisualComponentElement
{
public:
    VisualCircle(int x, int y, QColor color);
    VisualCircle::VisualCircle(VisualCircle *circle);
    ~VisualCircle(){}
    int radius;
    QColor fillColor;
    int lineThickness;
};

#endif // VISUALCIRCLE_H
