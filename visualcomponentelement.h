#include <QColor>
#include <QPoint>

#ifndef VISUALCOMPONENTELEMENT_H
#define VISUALCOMPONENTELEMENT_H

class VisualComponentElement
{
public:
    VisualComponentElement(int x, int y, QColor color);
    VisualComponentElement(VisualComponentElement *element);
    virtual ~VisualComponentElement(){}
    int x;
    int y;
    QColor mainColor;
};

#endif // VISUALCOMPONENTELEMENT_H
