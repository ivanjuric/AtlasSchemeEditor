#include "schemeeditormainwindow.h"
#include "ui_schemeeditormainwindow.h"
#include "libraryfile.h"
#include "regularbusview.h"
#include "popupnumericvalue.h"
#include "popupenumeratedvalue.h"

#include <QFileDialog>
#include <QToolButton>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QWheelEvent>
#include <QContextMenuEvent>
#include <QMessageBox>
#include "qgraphicsitem.h"

SchemeEditorMainWindow::SchemeEditorMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SchemeEditorMainWindow)
{
    conn = 0;
    ui->setupUi(this);
    createActions();

    // Create new Scene add initial components to it
    scene = new QGraphicsScene();
    scene->installEventFilter(this);
    ui->graphicsView->setScene(scene);

    //temp
    QString filePath = "C:\\Users\\Ivan\\Desktop\\FRISC_LIBRARY_SIMPLE.json";
    library = new LibraryFile(filePath);
    fillToolbar();

    createContextMenus();

}

void SchemeEditorMainWindow::createContextMenus()
{
    // Create context menus
    componentContextMenu = new QMenu(this);
    busContextMenu = new QMenu(this);
    connectionContextMenu = new QMenu(this);

    //Create actions
    actionMirrorComponent = new QAction(tr("&Mirror"), this);
    actionDeleteItem = new QAction(tr("&Delete"), this);


    // Create signal mappers and connect actions to them



    // Add actions to context menu
    componentContextMenu->addAction(actionMirrorComponent);
    componentContextMenu->addAction(actionDeleteItem);


//    contextMenuSignalMapper->setMapping(actionMirror,component->instanceName());
//    connect(actionMirror, SIGNAL(triggered()), contextMenuSignalMapper, SLOT(map()));
}

void SchemeEditorMainWindow::selectLibrary()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open config file"),"",tr("Config JSON file (*.json)"));

    LibraryFile *lib = new LibraryFile(fileName);
    if(lib->filePath != "")
    {
        library = lib;
        clearSceneView();
        ui->toolBar->clear();
        fillToolbar();
        this->setWindowTitle(library->libraryTitle);
    }
}
// Add selected component to graphics scene
void SchemeEditorMainWindow::AddComponentToScene(QString id)
{
    ComponentModel *model = library->getComponentById(id);
    if(model == 0)
        return;

    int numberOfSameComponents = getNumberOfSameComponentsInScene(model->instanceNameBase());
    if(model->maxInstances() != 0 && numberOfSameComponents >= model->maxInstances())
        return;

    // Create new component and rewrite importnant properties
    ComponentView *c = new ComponentView(model, QPoint(0,0));
    int instanceNameIndex = findComponentNameIndex(c->instanceNameBase());
    QString instanceName = createUniqueInstanceName(c->instanceNameBase(), instanceNameIndex);
    //c->setUniqueInstanceName(instanceNameIndex);
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

//    mirrorComponentSignalMapper->setMapping(actionMirrorComponent, c->instanceName());
//    deleteItemSignalMapper->setMapping(actionDeleteItem, c->instanceName());
    
    foreach(Attribute *attr, c->attributes())
    {
        if(attr->popupType() != PopupTypeEnum::Automatic)
            continue;
        showEditAttribute(attr);
    }

    scene->addItem(c);
}
void SchemeEditorMainWindow::showEditAttribute(Attribute *attribute)
{
    if(attribute->enumeratedValue().count() > 0)
    {
        PopupEnumeratedValue *enumPopup = new PopupEnumeratedValue();
        enumPopup->setScene(scene);
        //enumPopup->setComponent(c);
        enumPopup->setComponentAttribute(attribute);
        enumPopup->fillValues(!attribute->isValueSet());
        enumPopup->exec();
    }
    else
    {
        PopupNumericValue *valPopup = new PopupNumericValue();
        //valPopup->setScene(scene);
        //valPopup->setComponent(c);
        valPopup->setComponentAttribute(attribute);
        valPopup->fillValues(!attribute->isValueSet());

        valPopup->exec();
    }
}

// Add selected bus to graphics scene
void SchemeEditorMainWindow::AddBusToScene(QString id)
{
    Bus *bus = library->GetBusByUniqueId(id);
    if(bus == 0 || isSameBusInScene(bus->id()))
        return;

    // Create new component and rewrite importnant properties
    RegularBusView *b = new RegularBusView((RegularBus*)bus, QPoint(0,0));

    int instanceNameIndex = findBusNameIndex(b->id());
    QString instanceName = createUniqueInstanceName(b->id(), instanceNameIndex);
    b->setInstanceName(instanceName);

    foreach(PinView *pin, b->busPins())
    {
        pin->setStartPosition();
        pin->setFlag(QGraphicsItem::ItemIsMovable,false);
        pin->setParentItem(b);
    }
    b->setLabel();
    scene->addItem(b);
}
void SchemeEditorMainWindow::fillToolbar()
{
    foreach (ComponentModel *c, library->componentList)
    {
        QAction *actionAddFromToolbar = new QAction(this);
        actionAddFromToolbar->setIcon(getIconPixmap(c->iconFile()));
        actionAddFromToolbar->setIconText(c->id());
        actionAddFromToolbar->setIconVisibleInMenu(true);

        componentsSignalMapper->setMapping(actionAddFromToolbar,c->id());
        connect(actionAddFromToolbar, SIGNAL(triggered()), componentsSignalMapper, SLOT(map()));
        ui->toolBar->addAction(actionAddFromToolbar);
    }

    ui->toolBar->addSeparator();

    foreach(Bus *bus, library->regularBuses)
    {
        QAction *actionAddFromToolbar = new QAction(bus->id(),this);
        busSignalMapper->setMapping(actionAddFromToolbar,bus->id());
        connect(actionAddFromToolbar, SIGNAL(triggered()), busSignalMapper, SLOT(map()));
        ui->toolBar->addAction(actionAddFromToolbar);
    }
}
QPixmap SchemeEditorMainWindow::getIconPixmap(QString iconFile)
{
    QFileInfo inf(iconFile);
    QDir dir = inf.dir();
    QString path = dir.toNativeSeparators(dir.absoluteFilePath(inf.fileName()));
    QPixmap pix(path);

    return pix;
}

void SchemeEditorMainWindow::clearSceneView()
{
    clearConnections();
    scene->clear();
    ui->graphicsView->viewport()->update();
}
void SchemeEditorMainWindow::clearConnections()
{
    foreach (QGraphicsItem *item, scene->items())
    {
        QGraphicsPathItem *c = dynamic_cast<QGraphicsPathItem*>(item);
        if(c)
            scene->removeItem(c);
    }
}

void SchemeEditorMainWindow::clearScene()
{
    clearSceneView();
}

void SchemeEditorMainWindow::saveSceneToFile()
{
    QString fname = QFileDialog::getSaveFileName();
    if (fname.isEmpty())
        return;

    QFile f(fname);
    f.open(QFile::WriteOnly);
    QDataStream ds(&f);
    save(ds);
}
void SchemeEditorMainWindow::loadSceneFromFile()
{
    QString fname = QFileDialog::getOpenFileName();
    if (fname.isEmpty())
        return;

    QFile f(fname);
    f.open(QFile::ReadOnly);
    QDataStream ds(&f);
    load(ds);
}
void SchemeEditorMainWindow::save(QDataStream &ds)
{
//    ds << library->filePath;
//    // Save all components and its pin children
//    foreach(QGraphicsItem *item, scene->items())
//    {
//        if (item->type() == ComponentView::Type)
//        {
//            ds << item->type();
//            ((ComponentView*) item)->save(ds);
//        }
//    }
}
void SchemeEditorMainWindow::load(QDataStream &ds)
{
//    scene->clear();
//    QMap<quint64, PinView*> pinMap;

//    QString libraryFilePath;
//    ds >> libraryFilePath;

//    library = new LibraryFile(libraryFilePath);

//    while (!ds.atEnd())
//    {
//        int type;
//        ds >> type;
//        if (type == ComponentView::Type)
//        {
//            ComponentModel *c = new ComponentModel();
//            ComponentView *component = new ComponentView(model, QPoint(0,0));
//            scene->addItem(component);
//            component->load(ds, pinMap);
//        }
//    }
}

// Podesavanje menija i toolbara
void SchemeEditorMainWindow::createActions()
{
    componentsSignalMapper = new QSignalMapper(this);
    connect(componentsSignalMapper, SIGNAL(mapped(QString)),this,SLOT(AddComponentToScene(QString)));

    contextMenuSignalMapper = new QSignalMapper(this);
    connect(contextMenuSignalMapper, SIGNAL(mapped(QString)),this,SLOT(mirror(QString)));
    
    // Context menu signal mappers
    componentContextMenuSignalMapper = new QSignalMapper(this);
    connect(componentContextMenuSignalMapper, SIGNAL(mapped(QString)),this,SLOT(createComponentContextMenu(QString)));
    



    mirrorComponentSignalMapper = new QSignalMapper(this);
    connect(mirrorComponentSignalMapper, SIGNAL(mapped(QString)), this, SLOT(mirror(QString)));

    deleteItemSignalMapper = new QSignalMapper(this);
    connect(deleteItemSignalMapper, SIGNAL(mapped(QString)), this, SLOT(deleteItem(QString)));

    editAttributesSignalMapper = new QSignalMapper(this);
    connect(editAttributesSignalMapper, SIGNAL(mapped(QString)), this, SLOT(editAttributes(QString)));





    busSignalMapper = new QSignalMapper(this);
    connect(busSignalMapper, SIGNAL(mapped(QString)),this,SLOT(AddBusToScene(QString)));

    connect(ui->actionSelectLibrary, SIGNAL(triggered()), this, SLOT(selectLibrary()));
    ui->mainToolBar->addAction(ui->actionSelectLibrary);

    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveSceneToFile()));
    ui->mainToolBar->addAction(ui->actionSave);

    connect(ui->actionLoad, SIGNAL(triggered()), this, SLOT(loadSceneFromFile()));
    ui->mainToolBar->addAction(ui->actionLoad);

    connect(ui->actionClearScene, SIGNAL(triggered()), this, SLOT(clearScene()));
    ui->mainToolBar->addAction(ui->actionClearScene);
}

void SchemeEditorMainWindow::createComponentContextMenu(QString instanceName)
{
    
}


// Check if component is already attached to graphics scene
bool SchemeEditorMainWindow::isComponentInScene(int uid)
{
//    QList<QGraphicsItem*> items = scene->items();
//    for(int i = 0; i < items.length(); i++){
//        ComponentView *c = dynamic_cast<ComponentView*>(items[i]);
//        if(c && c->uid() == uid)
//            return true;
//    }
    return false;
}
// Check if component is already attached to graphics scene
bool SchemeEditorMainWindow::isSameBusInScene(QString id)
{
    QList<QGraphicsItem*> items = scene->items();
    for(int i = 0; i < items.length(); i++){
        RegularBusView *b = dynamic_cast<RegularBusView*>(items[i]);
        if(b && b->id() == id)
            return true;
    }
    return false;
}

SchemeEditorMainWindow::~SchemeEditorMainWindow()
{
    delete ui;
}

QGraphicsItem* SchemeEditorMainWindow::itemAt(const QPointF &pos)
{
    QList<QGraphicsItem*> items = scene->items(QRectF(pos - QPointF(1,1), QSize(3,3)));

    foreach(QGraphicsItem *item, items)
        if (item->type() > QGraphicsItem::UserType)
            return item;

    return 0;
}

bool SchemeEditorMainWindow::eventFilter(QObject *o, QEvent *e)
{
    QGraphicsSceneMouseEvent *me = (QGraphicsSceneMouseEvent*) e;
    switch ((int) e->type())
    {
        case QEvent::GraphicsSceneMousePress:
        {
            switch ((int) me->button())
            {
                case Qt::LeftButton:
                {
                    QGraphicsItem *item = itemAt(me->scenePos());

                    if(item == 0)
                        return false;

                    if (item->type() == PinView::Type)
                    {
                        PinView *pin1 = (PinView*)item;
                        if(pin1->side() == PinSideEnum::None)
                            return false;
                        conn = new Connection(0);
                        scene->addItem(conn);

                        conn->setPin1(pin1);
                        QPointF startP = pin1->scenePos();
                        startP.setX(startP.x() + pin1->width()/2);
                        startP.setY(startP.y() + pin1->height()/2);
                        conn->setPos1(startP);
                        conn->setPos2(me->scenePos());
                        conn->updatePath();

                        return true;
                    }
                    else if (item->type() == ComponentView::Type)
                    {
                        /* if (selBlock)
                            selBlock->setSelected(); */
                        // selBlock = (QNEBlock*) item;
                    }
                    break;
                }
                case Qt::RightButton:
                {
                    activeItem = 0;
                    QGraphicsItem *item = itemAt(me->scenePos());
                    if (item == 0)
                        return false;

                    QPointF pointF = me->scenePos();
                    QPoint *point = new QPoint();
                    point->setX((int)pointF.x());
                    point->setY((int)pointF.y());

                    if((item->type() == Connection::Type || item->type() == ComponentView::Type) || item->type() == RegularBusView::Type)
                    {
                        activeItem = item;
                        //deleteItem(item);
                        QContextMenuEvent *ev = new QContextMenuEvent(QContextMenuEvent::Mouse, *point,QCursor::pos());
                        QApplication::postEvent(this,ev);
                    }


                    // if (selBlock == (QNEBlock*) item)
                        // selBlock = 0;
                    break;
                }
            }
        }
        case QEvent::GraphicsSceneMouseMove:
        {
            if (conn)
            {
                conn->setPos2(me->scenePos());
                conn->updatePath();
                return true;
            }
            break;
        }
        case QEvent::GraphicsSceneMouseRelease:
        {
            if (conn && me->button() == Qt::LeftButton)
            {
                QGraphicsItem *item = itemAt(me->scenePos());
                if (item && item->type() == PinView::Type)
                {
                    PinView *pin1 = conn->pin1();
                    PinView *pin2 = (PinView*) item;

                    if(!isConnectionAllowed(pin1,pin2))
                    {
                        delete conn;
                        conn = 0;
                        return true;
                    }


                    // Create connection between component and bus
                    if(pin1->parentComponent() && pin2->parentBus())
                    {
                        if(checkRegularBusRule(pin1, pin2))
                        {
                            conn->setPos2(pin2->centerPos((PinView*)item));
                            conn->setPin2(pin2);
                            conn->updatePath();
                            conn = 0;
                            return true;
                        }
                    }
                    // Create connection between two components and create automatic bus
                    else if (pin1->parentComponent() && pin2->parentComponent())
                    {
                        QString busID = checkAutomaticBusRule(pin1,pin2);
                        if(busID != "")
                        {
                            AutomaticBus *a = library->getAutomaticBusById(busID);
                            if(a)
                            {
                                QVector<AutomaticBus*> ab = getAutomaticBusesFromScene();
                                int num = ab.length() + 1;
                                QString instanceName = createUniqueInstanceName(a->id(), num);
                                a->setInstanceName(instanceName);
                                conn->setAutomaticBus(a);
                                conn->setPos2(pin2->centerPos((PinView*)item));
                                conn->setPin2(pin2);
                                conn->updatePath();
                                conn = 0;
                                return true;
                            }
                        }
                    }
                }

                delete conn;
                conn = 0;
                return true;
            }
            break;
        }
    }
    return QObject::eventFilter(o, e);
}


bool SchemeEditorMainWindow::isConnectionAllowed(PinView *pin1, PinView *pin2)
{
    if(!(pin1 && pin2))
        return false;
    if (pin1->parentComponent() == pin2->parentComponent())
        return false;
    if (pin1->isConnected() || pin2->isConnected())
        return false;

    return true;
}
bool SchemeEditorMainWindow::checkRegularBusRule(PinView *pin1, PinView *pin2)
{
    ComponentView *component = dynamic_cast<ComponentView*>(pin1->parentComponent());
    RegularBusView *bus = dynamic_cast<RegularBusView*>(pin2->parentBus());

    if(!(component && bus))
        return false;

    foreach(RegularBusConnectionRule *rule, library->regularBusConnectionRules)
    {
        if(rule->componentId() == component->id() && rule->busId() == bus->id() && rule->componentVisualPinId() == pin1->id())
            return true;
    }
    return false;
}
QString SchemeEditorMainWindow::checkAutomaticBusRule(PinView *pin1, PinView *pin2)
{
    ComponentView *component1 = dynamic_cast<ComponentView*>(pin1->parentComponent());
    ComponentView *component2 = dynamic_cast<ComponentView*>(pin2->parentComponent());

    if(!(pin1 && pin2 && component1 && component2))
        return false;

    foreach(AutomaticBusConnectionRule *rule, library->automaticBusConnectionRules)
    {
        if(!(rule->firstComponent() && rule->secondComponent()))
            continue;

        QString c1 = rule->firstComponent()->componentId();
        QString c2 = rule->secondComponent()->componentId();
        QString p1 = rule->firstComponent()->componentVisualPinId();
        QString p2 = rule->secondComponent()->componentVisualPinId();

        if(c1 == component1->id() && c2 == component2->id() && p1 == pin1->id() && p2 == pin2->id()
           || (c2 == component1->id() && c1 == component2->id() && p2 == pin1->id() && p1 == pin2->id()))
            return rule->busId();
    }
    return "";
}
void SchemeEditorMainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    if(event->reason() == QContextMenuEvent::Mouse)
    {

        ComponentView *component = dynamic_cast<ComponentView*>(activeItem);
        if(component)
        {
            QAction *actionMirror = new QAction(tr("&Mirror"), this);
            mirrorComponentSignalMapper->setMapping(actionMirror, component->instanceName());
            connect(actionMirror, SIGNAL(triggered()), mirrorComponentSignalMapper, SLOT(map()));

            QAction *actionDeleteItem = new QAction(tr("&Delete"), this);
            deleteItemSignalMapper->setMapping(actionDeleteItem, component->instanceName());
            connect(actionDeleteItem, SIGNAL(triggered()), deleteItemSignalMapper, SLOT(map()));

            QAction *actionEditAttributes = new QAction(tr("&Edit attributes"), this);
            editAttributesSignalMapper->setMapping(actionEditAttributes, component->instanceName());
            connect(actionEditAttributes, SIGNAL(triggered()), editAttributesSignalMapper, SLOT(map()));



            QMenu *contextMenu = new QMenu(this);
            contextMenu->addAction(actionMirror);
            contextMenu->addAction(actionDeleteItem);
            contextMenu->addAction(actionEditAttributes);
            contextMenu->exec(event->globalPos());


            //componentContextMenu->exec(event->globalPos());
        }
        Connection *connection = dynamic_cast<Connection*>(activeItem);
        if(connection)
        {
        }
    }
}

void SchemeEditorMainWindow::mirror(QString instanceName)
{
    ComponentView *c = getComponentFromScene(instanceName);
    if(c)
    {
        c->mirrorSides();
        scene->update();
        ui->graphicsView->update();
        updateConnections();
    }
}
void SchemeEditorMainWindow::deleteItem(QString instanceName)
{
    ComponentView *c = getComponentFromScene(instanceName);
    if(c)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Delete item");
        msgBox.setText("Are you sure you want to delete this item?");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        int ret = msgBox.exec();
        if(ret == QMessageBox::Ok)
        {
            QString name = c->instanceName();
            //scene->removeItem(c);
            removeItemFromScene(name);
            scene->update();
            ui->graphicsView->update();
            //updateConnections();
        }
    }

    scene->update();
    ui->graphicsView->update();
    ui->graphicsView->viewport()->update();
}
void SchemeEditorMainWindow::editAttributes(QString instanceName)
{
    ComponentView *c = getComponentFromScene(instanceName);
    if(c)
    {
        foreach(Attribute *attr, c->attributes())
        {
            showEditAttribute(attr);
        }
    }
}

void SchemeEditorMainWindow::removeItemFromScene(QString instanceName)
{
    QString name = instanceName;
    for(int i = 0; i < scene->items().length(); i++)
    {
        ComponentView *c = dynamic_cast<ComponentView*>(scene->items()[i]);
        if(c && c->instanceName() == name)
        {
            deleteConnections(c);
            scene->removeItem(scene->items()[i]);
            return;
        }
    }
}

ComponentView* SchemeEditorMainWindow::getComponentFromScene(QString instanceName)
{
    QList<QGraphicsItem*> items = scene->items();

    for(int i = 0; i < items.length(); i++)
    {
        ComponentView *c = dynamic_cast<ComponentView*>(items[i]);
        if(c && c->instanceName() == instanceName)
            return c;
    }
    return 0;
}

int SchemeEditorMainWindow::getNumberOfSameComponentsInScene(QString baseName)
{
    QList<QGraphicsItem*> items = scene->items();

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
int SchemeEditorMainWindow::getNumberOfSameBusesInScene(QString id)
{
    QList<QGraphicsItem*> items = scene->items();

    int total = 0;
    for(int i = 0; i < items.length(); i++)
    {
        RegularBusView *b = dynamic_cast<RegularBusView*>(items[i]);
        if(b && b->id().toLower() == id.toLower())
            total++;
    }
    return total;
}

int SchemeEditorMainWindow::findComponentNameIndex(QString baseName)
{
    QList<QGraphicsItem*> items = scene->items();

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
int SchemeEditorMainWindow::findBusNameIndex(QString id)
{
    QList<QGraphicsItem*> items = scene->items();

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

void SchemeEditorMainWindow::updateConnections()
{
    foreach (QGraphicsItem *item, scene->items())
    {
        Connection *c = dynamic_cast<Connection*>(item);
        if(c)
            c->updatePath();
    }
}
void SchemeEditorMainWindow::deleteConnections(QGraphicsItem *parent)
{
    ComponentView *comp = dynamic_cast<ComponentView*>(parent);
    foreach (QGraphicsItem *item, scene->items())
    {
        if(comp)
        {
            Connection *c = dynamic_cast<Connection*>(item);
            if(c && isConnectionFromPin(c, comp->instanceName()))
                delete item;

        }
    }
}

bool SchemeEditorMainWindow::isConnectionFromPin(Connection *connection, QString parentComponentInstanceName)
{
    QString pin1Name;
    QString pin2Name;

    if(connection && connection->pin1() && connection->pin2() && connection->pin1()->parentComponent() && connection->pin2()->parentComponent())
    {
        pin1Name = connection->pin1()->parentInstanceName();
        pin2Name = connection->pin2()->parentInstanceName();

        if(pin1Name == parentComponentInstanceName || pin2Name == parentComponentInstanceName)
            return true;
    }
    return false;
}

QString SchemeEditorMainWindow::createUniqueInstanceName(QString name, int num = 0)
{
    if(num > 0)
        return QString(name + "_" + QString::number(num).rightJustified(3,'0'));
    else
        return name;
}
QVector<Connection*> SchemeEditorMainWindow::getConnectionsFromScene()
{
    QVector<Connection*> conns;

    for(int i = 0; i < scene->items().length(); i++)
    {
        Connection *c = dynamic_cast<Connection*>(scene->items()[i]);
        if(c)
            conns.append(c);
    }

    return conns;
}
QVector<AutomaticBus*> SchemeEditorMainWindow::getAutomaticBusesFromScene()
{
    QVector<AutomaticBus*> automaticBuses;

    for(int i = 0; i < scene->items().length(); i++)
    {
        Connection *c = dynamic_cast<Connection*>(scene->items()[i]);
        if(c && c->automaticBus())
            automaticBuses.append(c->automaticBus());
    }

    return automaticBuses;
}


//void SchemeEditorMainWindow::dragEnterEvent(QDragEnterEvent *event)
//{

//}
//void SchemeEditorMainWindow::dragLeaveEvent(QDragLeaveEvent *event)
//{

//}
//void SchemeEditorMainWindow::dragMoveEvent(QDragMoveEvent *event)
//{

//}
//void SchemeEditorMainWindow::dropEvent(QDropEvent *event)
//{

//}

