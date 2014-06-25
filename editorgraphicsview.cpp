#include "componentview.h"
#include "editorgraphicsview.h"
#include <QWheelEvent>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>

EditorGraphicsView::EditorGraphicsView(QWidget* parent) : QGraphicsView(parent)
{
    //setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setRenderHints(QPainter::Antialiasing);
    //Use ScrollHand Drag Mode to enable Panning
//    setDragMode(ScrollHandDrag);
//    setCursor(Qt::ArrowCursor);

    currentScale = 1.0;
}

//void EditorGraphicsView::enterEvent(QEvent *event)
//{
//    QGraphicsView::enterEvent(event);
//    viewport()->setCursor(Qt::ArrowCursor);
//}

//void EditorGraphicsView::mousePressEvent(QMouseEvent *event)
//{
//    QGraphicsView::mousePressEvent(event);
//    viewport()->setCursor(Qt::ArrowCursor);
//}

//void EditorGraphicsView::mouseReleaseEvent(QMouseEvent *event)
//{
//    QGraphicsView::mouseReleaseEvent(event);
//    viewport()->setCursor(Qt::ArrowCursor);
//}
void EditorGraphicsView::wheelEvent(QWheelEvent* event) {

    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    // Scale the view / do the zoom
    double scaleFactor = 1.15;
    const double minFactor = 0.5;
    const double maxFactor = 4.0;
    if(event->delta() > 0)
    {
        // Zoom in
        if(currentScale <= maxFactor)
        {
            currentScale *= scaleFactor;
            scale(scaleFactor, scaleFactor);
        }
    }
    else
    {
        // Zooming out
        if(currentScale >= minFactor)
        {
            currentScale /= scaleFactor;
            scale(1.0 / scaleFactor, 1.0 / scaleFactor);
        }
    }
}

ComponentView* EditorGraphicsView::getComponentFromScene(QString name)
{
    QList<QGraphicsItem*> items = this->scene()->items();

    for(int i = 0; i < items.length(); i++)
    {
        ComponentView *c = dynamic_cast<ComponentView*>(items[i]);
        if(c && c->instanceName() == name)
            return c;
    }
    return 0;
}

//void EditorGraphicsView::contextMenuEvent(QContextMenuEvent *event)
//{
//    QPoint p = QCursor::pos();
//    QPointF p1 = event->pos();

//    QGraphicsItem* item = itemAt(QPointF(p.x(),p.y()));
//    QList<QGraphicsItem*> item1 = scene()->items();

//    if(item)
//    {
//        ComponentView *c = (ComponentView*)item;
//    }

//    createActions();
//    QMenu menu(this);
//    menu.addAction(actionMirror);
//    menu.exec(event->globalPos());
//}
//void EditorGraphicsView::createActions()
//{
//    actionMirror = new QAction(tr("&Mirror"), this);
//    //actionMirror->setShortcuts(QKeySequence::New);
//    actionMirror->setStatusTip(tr("Mirror component pins"));
//    connect(actionMirror, SIGNAL(triggered()), this, SLOT(mirror()));
//}

//void EditorGraphicsView::mirror()
//{

//}

//QGraphicsItem* EditorGraphicsView::itemAt(const QPointF &pos)
//{
//    QList<QGraphicsItem*> items = this->scene()->items(QRectF(pos - QPointF(1,1), QSize(3,3)));

//    QList<QGraphicsItem*> items1 = this->scene()->items();

//    foreach(QGraphicsItem *item, items)
//        if (item->type() > QGraphicsItem::UserType)
//            return item;

//    return 0;
//}


