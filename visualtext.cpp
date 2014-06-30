#include "visualtext.h"

VisualText::VisualText(int x, int y, QColor color)
    : VisualComponentElement(x, y, color)
{
    showInstanceName = false;
}

VisualText::VisualText(VisualText *text)
: VisualComponentElement(text->x, text->y, text->mainColor)
{
    this->showInstanceName = text->showInstanceName;
    this->text = text->text;
}
