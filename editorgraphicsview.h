#include <QGraphicsView>

#ifndef EDITORGRAPHICSVIEW_H
#define EDITORGRAPHICSVIEW_H

class EditorGraphicsView : public QGraphicsView
{
public:
    EditorGraphicsView(QWidget* parent = NULL);
protected:
    //Take over the interaction
    virtual void wheelEvent(QWheelEvent* event);
};

#endif // EDITORGRAPHICSVIEW_H
