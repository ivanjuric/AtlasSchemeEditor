#include <QColor>
#include <QPoint>

#ifndef VISUALCOMPONENTELEMENT_H
#define VISUALCOMPONENTELEMENT_H

class VisualComponentElement
{
public:
    VisualComponentElement(int x, int y, QColor color);
    int x;
    int y;
    QColor mainColor;
    virtual QPoint getPosition();
};

#endif // VISUALCOMPONENTELEMENT_H
