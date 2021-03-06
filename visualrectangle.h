#include "visualcomponentelement.h"

#ifndef VISUALRECTANGLE_H
#define VISUALRECTANGLE_H

#pragma once


class VisualRectangle : public VisualComponentElement
{
public:
    VisualRectangle(int x, int y, QColor color);
    VisualRectangle(VisualRectangle *rect);
    ~VisualRectangle(){}
    int width;
    int height;
    int lineThickness;
    QColor fillColor;
};

#endif // VISUALRECTANGLE_H
