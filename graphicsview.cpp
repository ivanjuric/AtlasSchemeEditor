#include "graphicsview.h"
#include <QWheelEvent>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>

 GraphicsView:: GraphicsView(QWidget* parent) : QGraphicsView(parent)
{
    //setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setRenderHints(QPainter::Antialiasing);
    //Use ScrollHand Drag Mode to enable Panning
//    setDragMode(ScrollHandDrag);
//    setCursor(Qt::ArrowCursor);

    setAcceptDrops(true);
    currentScale = 1.0;

    m_component = 0;
    m_bus = 0;
}
void  GraphicsView::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain"))
            event->acceptProposedAction();
}
void  GraphicsView::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain"))
            event->acceptProposedAction();
}
void  GraphicsView::dropEvent(QDropEvent *event)
{
    QString id = event->mimeData()->text();

    if(component() && component()->id() == id)
    {
        //ComponentView *c = new ComponentView(component());
        component()->setPos(event->posF());
        scene()->addItem(component());
    }
    else if(bus() && bus()->id() == id)
    {
        //RegularBusView *b = new RegularBusView(bus());
        bus()->setPos(event->posF());
        scene()->addItem(bus());
    }

    setComponent(0);
    setBus(0);


    event->acceptProposedAction();
}

void  GraphicsView::wheelEvent(QWheelEvent* event) {

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

ComponentView*  GraphicsView::getComponentFromScene(QString name)
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

//void GraphicsView::contextMenuEvent(QContextMenuEvent *event)
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
//void GraphicsView::createActions()
//{
//    actionMirror = new QAction(tr("&Mirror"), this);
//    //actionMirror->setShortcuts(QKeySequence::New);
//    actionMirror->setStatusTip(tr("Mirror component pins"));
//    connect(actionMirror, SIGNAL(triggered()), this, SLOT(mirror()));
//}

//void GraphicsView::mirror()
//{

//}

//QGraphicsItem* GraphicsView::itemAt(const QPointF &pos)
//{
//    QList<QGraphicsItem*> items = this->scene()->items(QRectF(pos - QPointF(1,1), QSize(3,3)));

//    QList<QGraphicsItem*> items1 = this->scene()->items();

//    foreach(QGraphicsItem *item, items)
//        if (item->type() > QGraphicsItem::UserType)
//            return item;

//    return 0;
//}


