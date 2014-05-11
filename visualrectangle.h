#include "visualcomponentelement.h"

#ifndef VISUALRECTANGLE_H
#define VISUALRECTANGLE_H

class VisualRectangle : public VisualComponentElement
{
public:
    VisualRectangle(int x, int y, QColor color);
    int width;
    int height;
    int lineThickness;
    QColor fillColor;
};

#endif // VISUALRECTANGLE_H
