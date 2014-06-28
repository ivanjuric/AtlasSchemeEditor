#include <QToolButton>

#ifndef TOOLBOXBUTTON_H
#define TOOLBOXBUTTON_H

class ToolboxButton : public QToolButton
{
public:
    ToolboxButton(QWidget *parent = 0);

private:
    void mousePressEvent(QMouseEvent *e);

};

#endif // TOOLBOXBUTTON_H
