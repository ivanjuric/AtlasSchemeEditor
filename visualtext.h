#include "visualcomponentelement.h"

#ifndef VISUALTEXT_H
#define VISUALTEXT_H

class VisualText : public VisualComponentElement
{
public:
    VisualText(int x, int y, QColor color);
    QString text;
};

#endif // VISUALTEXT_H
