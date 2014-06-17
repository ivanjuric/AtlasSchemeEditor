#include "visualtext.h"

VisualText::VisualText(int x, int y, QColor color)
    : VisualComponentElement(x, y, color)
{

}

VisualText::VisualText(VisualText *text)
: VisualComponentElement(text->x, text->y, text->mainColor)
{
    this->text = text->text;
}
