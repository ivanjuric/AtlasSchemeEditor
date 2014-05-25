#include "visualcomponentelement.h"

#ifndef VISUALCIRCLE_H
#define VISUALCIRCLE_H

class VisualCircle  : public VisualComponentElement
{
public:
    VisualCircle(int x, int y, QColor color);
    int radius;
    QColor fillColor;
    int lineThickness;
};

#endif // VISUALCIRCLE_H
