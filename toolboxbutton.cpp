#include "toolboxbutton.h"

ToolboxButton::ToolboxButton(QWidget *parent) : QToolButton(parent)
{
}
void ToolboxButton::mousePressEvent(QMouseEvent *e)
{
    pressed();
}
