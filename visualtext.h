#include "visualcomponentelement.h"

#ifndef VISUALTEXT_H
#define VISUALTEXT_H

#pragma once


class VisualText : public VisualComponentElement
{
public:
    VisualText(int x, int y, QColor color);
    VisualText::VisualText(VisualText *text);
    ~VisualText(){}
    QString text;
};

#endif // VISUALTEXT_H
