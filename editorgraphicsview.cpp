#include "editorgraphicsview.h"
#include <QWheelEvent>

EditorGraphicsView::EditorGraphicsView(QWidget* parent) : QGraphicsView(parent)
{
    //setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    //Use ScrollHand Drag Mode to enable Panning
    //setDragMode(ScrollHandDrag);
}
void EditorGraphicsView::wheelEvent(QWheelEvent* event) {

    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    // Scale the view / do the zoom
    double scaleFactor = 1.15;
    if(event->delta() > 0) {
        // Zoom in
        scale(scaleFactor, scaleFactor);
    }
    else {
        // Zooming out
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
}
