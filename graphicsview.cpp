#include "graphicsview.h"
#include "popupenumeratedvalue.h"
#include "popupnumericvalue.h"
#include <QWheelEvent>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>

GraphicsView::GraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    setRenderHints(QPainter::Antialiasing);
    setAcceptDrops(true);
    setAlignment(Qt::AlignCenter);
    // Remove setting scene rect to enable scroll bars
    //setSceneRect(rect());

    setCurrentScale(1.0);
}

ComponentView* GraphicsView::getComponentFromScene(QString name)
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
ComponentView* GraphicsView::createComponent(QString id, QPointF pos)
{
    ComponentModel *model = library()->getComponentById(id);
    if(model == 0)
        return 0;

    int numberOfSameComponents = getNumberOfSameComponentsInScene(model->instanceNameBase());
    if(model->maxInstances() != 0 && numberOfSameComponents >= model->maxInstances())
        return 0;

    // Create new component and rewrite importnant properties
    ComponentView *c = new ComponentView(model);
    int instanceNameIndex = findComponentNameIndex(c->instanceNameBase());
    QString instanceName = createUniqueInstanceName(c->instanceNameBase(), instanceNameIndex);
    c->setInstanceName(instanceName);
    c->setParentToPins();
    foreach(PinView *pin, c->pins())
    {
        pin->setStartPosition();
        pin->setFlag(QGraphicsItem::ItemIsMovable,false);
        pin->setLabel();
        pin->setParentItem(c);
    }
    c->setMirrored(false);

    foreach(Attribute *attr, c->attributes())
    {
        if(attr->popupType() != PopupTypeEnum::Automatic)
            continue;
        showEditAttribute(attr);
    }
    c->setPos(pos);

    return c;
}
int GraphicsView::getNumberOfSameComponentsInScene(QString baseName)
{
    QList<QGraphicsItem*> items = scene()->items();

    int total = 0;
    for(int i = 0; i < items.length(); i++)
    {
        ComponentView *c = dynamic_cast<ComponentView*>(items[i]);
        //ComponentView *c = (ComponentView*)items[i];
        if(c && c->instanceNameBase().toLower() == baseName.toLower())
            total++;
    }
    return total;
}
int GraphicsView::findComponentNameIndex(QString baseName)
{
    QList<QGraphicsItem*> items = scene()->items();

    int max = 0;
    for(int i = 0; i < items.length(); i++)
    {
        ComponentView *c = dynamic_cast<ComponentView*>(items[i]);
        if(c && c->instanceNameBase().toLower() == baseName.toLower())
        {
            QStringList splitArray = c->instanceName().split('_');
            if(splitArray.length() == 2 && splitArray[1].toInt() > max)
                max = splitArray[1].toInt();
        }
    }
    int existingComponents = getNumberOfSameComponentsInScene(baseName);
    if(existingComponents > max)
        max = existingComponents;

    return max + 1;
}
void GraphicsView::showEditAttribute(Attribute *attribute)
{
    if(attribute->enumeratedValue().count() > 0)
    {
        PopupEnumeratedValue *enumPopup = new PopupEnumeratedValue();
        enumPopup->setScene(scene());
        enumPopup->setComponentAttribute(attribute);
        enumPopup->fillValues(!attribute->isValueSet());
        enumPopup->exec();
    }
    else
    {
        PopupNumericValue *valPopup = new PopupNumericValue();
        valPopup->setComponentAttribute(attribute);
        valPopup->fillValues(!attribute->isValueSet());

        valPopup->exec();
    }
}


RegularBusView* GraphicsView::createBus(QString id, QPointF pos)
{
    Bus *bus = library()->GetBusByUniqueId(id);
    if(bus == 0 || isSameBusInScene(bus->id()))
        return 0;

    // Create new component and rewrite importnant properties
    RegularBusView *b = new RegularBusView((RegularBus*)bus);

    int instanceNameIndex = findBusNameIndex(b->id());
    QString instanceName = createUniqueInstanceName(b->id(), instanceNameIndex);
    b->setInstanceName(instanceName);

    foreach(PinView *pin, b->busPins())
    {
        pin->setStartPosition();
        pin->setFlag(QGraphicsItem::ItemIsMovable,false);
        pin->setParentItem(b);
        pin->setParentInstanceName(b->instanceName());
    }
    b->setLabel();
    b->setPos(pos);

    return b;
}
bool GraphicsView::isSameBusInScene(QString id)
{
    QList<QGraphicsItem*> items = scene()->items();
    for(int i = 0; i < items.length(); i++){
        RegularBusView *b = dynamic_cast<RegularBusView*>(items[i]);
        if(b && b->id() == id)
            return true;
    }
    return false;
}
int GraphicsView::findBusNameIndex(QString id)
{
    QList<QGraphicsItem*> items = scene()->items();

    int max = 0;
    for(int i = 0; i < items.length(); i++)
    {
        RegularBusView *b = dynamic_cast<RegularBusView*>(items[i]);
        if(b && b->id().toLower() == id.toLower())
        {
            QStringList splitArray = b->instanceName().split('_');
            if(splitArray.length() == 2 && splitArray[1].toInt() > max)
                max = splitArray[1].toInt();
        }
    }
    int existingComponents = getNumberOfSameBusesInScene(id);
    if(existingComponents > max)
        max = existingComponents;

    return max + 1;
}
int GraphicsView::getNumberOfSameBusesInScene(QString id)
{
    QList<QGraphicsItem*> items = scene()->items();

    int total = 0;
    for(int i = 0; i < items.length(); i++)
    {
        RegularBusView *b = dynamic_cast<RegularBusView*>(items[i]);
        if(b && b->id().toLower() == id.toLower())
            total++;
    }
    return total;
}



QString GraphicsView::createUniqueInstanceName(QString name, int num)
{
    if(num > 0)
        return QString(name + "_" + QString::number(num).rightJustified(3,'0'));
    else
        return name;
}




void GraphicsView::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain"))
            event->acceptProposedAction();
}
void GraphicsView::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain"))
            event->acceptProposedAction();
}
void GraphicsView::dropEvent(QDropEvent *event)
{
    QString id = event->mimeData()->text();
    ComponentView *c = createComponent(id, mapToScene(event->pos()));
    RegularBusView *b = createBus(id, mapToScene(event->pos()));
    if(c && c->id() == id)
        scene()->addItem(c);
    else if(b && b->id() == id)
        scene()->addItem(b);

    event->acceptProposedAction();
}

void GraphicsView::wheelEvent(QWheelEvent* event)
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    // Scale the view / do the zoom
    double scaleFactor = 1.15;
    const double minFactor = 0.5;
    const double maxFactor = 4.0;
    if(event->delta() > 0)
    {
        // Zoom in
        if(currentScale() <= maxFactor)
        {
            setCurrentScale(currentScale() * scaleFactor);
            scale(scaleFactor, scaleFactor);
        }
    }
    else
    {
        // Zooming out
        if(currentScale() >= minFactor)
        {
            setCurrentScale(currentScale() / scaleFactor);
            scale(1.0 / scaleFactor, 1.0 / scaleFactor);
        }
    }
}

