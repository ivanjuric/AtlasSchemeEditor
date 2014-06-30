#include "schemeeditormainwindow.h"
#include "ui_schemeeditormainwindow.h"
#include "libraryfile.h"
#include "regularbusview.h"
#include "popupnumericvalue.h"
#include "popupenumeratedvalue.h"
#include "toolboxbutton.h"

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
    //setAcceptDrops(true);
    conn = 0;
    ui->setupUi(this);
    createActions();

    // Create new Scene add initial components to it
    scene = new QGraphicsScene();
    scene->installEventFilter(this);
    ui->graphicsView->setScene(scene);

    //temp
    QString filePath = "C:\\Users\\Ivan\\Desktop\\FRISC_LIBRARY_SIMPLE.json";
    library = new LibraryFile();
    library->loadJson(filePath);
    ui->graphicsView->setLibrary(library);
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
}

void SchemeEditorMainWindow::selectLibrary()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open config file"),"",tr("Config JSON file (*.json)"));

    library = new LibraryFile();
    if(library->loadJson(fileName))
    {
        ui->graphicsView->setLibrary(library);
        clearSceneView();
        ui->toolBar->clear();
        fillToolbar();
        this->setWindowTitle(library->libraryTitle);
    }
}
ComponentView* SchemeEditorMainWindow::createComponentViewFromFile(QString id, bool mirrored, QString instanceName, QPointF pos, QMap<QString,int> attributes)
{
    ComponentModel *model = library->getComponentById(id);
    ComponentView *c = new ComponentView(model);
    c->setInstanceName(instanceName);
    c->setParentToPins();
    foreach(PinView *pin, c->pins())
    {
        pin->setStartPosition();
        pin->setFlag(QGraphicsItem::ItemIsMovable,false);
        pin->setLabel();
        pin->setParentItem(c);
    }
    c->setMirrored(mirrored);
    if(c->mirrored())
        c->mirrorSides();

    foreach(Attribute *a, c->attributes())
    {
        a->setCurrentValue(attributes[a->id()]);
    }
    c->setPos(pos);
    return c;
}



// Add selected bus to graphics scene
void SchemeEditorMainWindow::AddBusToScene(QString id)
{
    Bus *bus = library->GetBusByUniqueId(id);
    if(bus == 0 || ui->graphicsView->isSameBusInScene(bus->id()))
        return;

    // Create new component and rewrite importnant properties
    RegularBusView *b = new RegularBusView((RegularBus*)bus);

    int instanceNameIndex = ui->graphicsView->findBusNameIndex(b->id());
    QString instanceName = ui->graphicsView->createUniqueInstanceName(b->id(), instanceNameIndex);
    b->setInstanceName(instanceName);

    foreach(PinView *pin, b->busPins())
    {
        pin->setStartPosition();
        pin->setFlag(QGraphicsItem::ItemIsMovable,false);
        pin->setParentItem(b);
        pin->setParentInstanceName(b->instanceName());
    }
    b->setLabel();
    b->setPos(QPointF(0,0));
    scene->addItem(b);
}
RegularBusView* SchemeEditorMainWindow::createRegularBusViewFromFile(QString id, QPointF pos, QString instanceName)
{
    Bus *bus = library->GetBusByUniqueId(id);
    RegularBusView *b = new RegularBusView((RegularBus*)bus);
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
Connection* SchemeEditorMainWindow::createConnectionFromFile(QString parentName1,QString pin1,QString parentName2,QString pin2,AutomaticBus *automaticBus)
{
    ComponentView *c1 = 0, *c2 = 0;
    RegularBusView *b = 0;
    PinView *p1 = 0, *p2 = 0;

    // Find first pin and its component parent
    foreach(ComponentView *comp, getComponentsFromScene())
    {
        if(comp->instanceName() == parentName1)
        {
            foreach (PinView *pin, comp->pins())
            {
                if(pin->id() == pin1)
                {
                    c1 = comp;
                    p1 = pin;
                    break;
                }
            }
            break;
        }
    }
    // Find second pin and its parent - bus or component
    if(automaticBus)
    {
        foreach(ComponentView *comp, getComponentsFromScene())
        {
            if(comp->instanceName() == parentName2)
            {
                foreach (PinView *pin, comp->pins())
                {
                    if(pin->id() == pin2)
                    {
                        c2 = comp;
                        p2 = pin;
                        break;
                    }
                }
                break;
            }
        }
    }
    else
    {
        foreach (RegularBusView *bus, getRegularBusesFromScene())
        {
            if(bus->instanceName() == parentName2)
            {
                foreach (PinView *pin, bus->busPins())
                {
                    if(pin->id() == pin2)
                    {
                        b = bus;
                        p2 = pin;
                        break;
                    }

                }
                break;
            }
        }
    }

    Connection *c = new Connection(0);
    c->setPin1(p1);
    c->setPos1(p1->centerPos(p1));
    c->setPin2(p2);
    c->setPos2(p2->centerPos(p2));
    if(automaticBus)
        c->setAutomaticBus(automaticBus);
    c->updatePath();
    return c;
}

void SchemeEditorMainWindow::fillToolbar()
{
    foreach (ComponentModel *c, library->componentList)
    {
        QAction *a = new QAction(this);
        a->setIcon(getIconPixmap(c->iconFile()));
        a->setIconText(c->id());
        a->setIconVisibleInMenu(true);
        ToolboxButton *btn = new ToolboxButton();
        btn->setDefaultAction(a);
        btn->setText(c->title());
        btn->setToolTip(c->tooltip());

        dragAndDropComponentSignalMapper->setMapping(btn,c->id());
        connect(btn, SIGNAL(pressed()), dragAndDropComponentSignalMapper, SLOT(map()));

        ui->toolBar->addWidget(btn);
    }

    ui->toolBar->addSeparator();

    foreach(RegularBus *b, library->regularBuses)
    {
        QAction *a = new QAction(this);
        a->setIcon(getIconPixmap(b->iconFile()));
        a->setIconText(b->id());
        a->setIconVisibleInMenu(true);
        ToolboxButton *btn = new ToolboxButton();
        btn->setDefaultAction(a);

        dragAndDropBusSignalMapper->setMapping(btn, b->id());
        connect(btn, SIGNAL(pressed()),dragAndDropBusSignalMapper, SLOT(map()));

        ui->toolBar->addWidget(btn);
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

    scene = new QGraphicsScene();
    scene->installEventFilter(this);
    ui->graphicsView->setScene(scene);

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
    QString fname = QFileDialog::getSaveFileName(this,tr("Save scene to file"),"",tr("Binary configuration (*.cfg)"));
    if (fname.isEmpty())
        return;

    QFile f(fname);
    f.open(QFile::WriteOnly);
    QDataStream ds(&f);
    save(ds);
}
void SchemeEditorMainWindow::loadSceneFromFile()
{
    QString fname = QFileDialog::getOpenFileName(this,tr("Load scene from file"),"",tr("Binary configuration (*.cfg)"));
    if (fname.isEmpty())
        return;

    clearSceneView();
    QFile f(fname);
    f.open(QFile::ReadOnly);
    QDataStream ds(&f);
    load(ds);
}
void SchemeEditorMainWindow::save(QDataStream &ds)
{
    // Save current library filename (file location is "/library")
    ds << library->libraryFileName;

    // Save components from scene
    QVector<ComponentView*> components = getComponentsFromScene();
    ds << components.length();
    foreach (ComponentView *c, components)
    {
        ds << c->id();
        ds << c->instanceName();
        ds << c->pos();
        ds << c->mirrored();
        ds << c->attributes().length();
        foreach(Attribute *a, c->attributes())
        {
            ds << a->id();
            ds << a->currentValue();
        }
    }
    // Save buses from scene
    QVector<RegularBusView*> buses = getRegularBusesFromScene();
    ds << buses.length();
    foreach (RegularBusView *b, buses)
    {
        ds << b->id();
        ds << b->instanceName();
        ds << b->pos();
    }

    // Save connections from scene
    QVector<Connection*> connections = getConnectionsFromScene();
    ds << connections.length();
    foreach(Connection *c, connections)
    {
        if(c->automaticBus())
        {
            ds << 1;
            ds << c->automaticBus()->id();
            ds << c->automaticBus()->instanceName();
        }
        else
        {
            ds << 0;
        }
        ds << c->pin1()->parentInstanceName();
        ds << c->pin1()->id();
        ds << c->pin2()->parentInstanceName();
        ds << c->pin2()->id();
    }
}
void SchemeEditorMainWindow::load(QDataStream &ds)
{
    scene->clear();

    // Load library used in saved configuration
    QString libraryFileName;
    ds >> libraryFileName;
    QFileInfo info(QDir::toNativeSeparators(QDir::currentPath() + "/library"), libraryFileName);

    library = new LibraryFile();
    library->loadJson(info.absoluteFilePath());
    ui->graphicsView->setLibrary(library);


    // Load components
    int componentNumber;
    ds >> componentNumber;
    for(int i=0; i < componentNumber; i++)
    {
        QString id;
        ds >> id;
        QString instanceName;
        ds >> instanceName;
        QPointF pos;
        ds >> pos;
        bool mirrored;
        ds >> mirrored;
        int attrNum;
        ds >> attrNum;
        QMap<QString,int> attributes;
        for(int i = 0; i < attrNum; i++)
        {
            QString attrId;
            ds >> attrId;
            int attrVal;
            ds >> attrVal;
            attributes[attrId] = attrVal;
        }
        ComponentView *c = createComponentViewFromFile(id,mirrored,instanceName,pos,attributes);
        scene->addItem(c);
    }

    // Load buses
    int busNumber;
    ds >> busNumber;
    for(int i=0; i < busNumber; i++)
    {
        QString id;
        ds >> id;
        QString instanceName;
        ds >> instanceName;
        QPointF pos;
        ds >> pos;
        RegularBusView *b = createRegularBusViewFromFile(id,pos,instanceName);
        scene->addItem(b);
    }

    // Load connections and automatic buses
    int connectionNumber;
    ds >> connectionNumber;
    for(int i=0; i < connectionNumber; i++)
    {
        int type;
        ds >> type;
        AutomaticBus *a = 0;
        if(type == 1)
        {
            QString autoId;
            ds >> autoId;
            QString autoInstanceName;
            ds >> autoInstanceName;

            a = new AutomaticBus();
            a->setId(autoId);
            a->setInstanceName(autoInstanceName);
        }
        QString id1,id2,instanceName1,instanceName2;

        ds >> instanceName1;
        ds >> id1;
        ds >> instanceName2;
        ds >> id2;

        Connection *c = createConnectionFromFile(instanceName1,id1,instanceName2,id2,a);
        scene->addItem(c);
    }
}

// Podesavanje menija i toolbara
void SchemeEditorMainWindow::createActions()
{
    contextMenuSignalMapper = new QSignalMapper(this);
    connect(contextMenuSignalMapper, SIGNAL(mapped(QString)),this,SLOT(mirror(QString)));
    
    // Context menu signal mappers
    componentContextMenuSignalMapper = new QSignalMapper(this);
    connect(componentContextMenuSignalMapper, SIGNAL(mapped(QString)),this,SLOT(createComponentContextMenu(QString)));
    

    // Drag signal mappers
    dragAndDropComponentSignalMapper = new QSignalMapper(this);
    connect(dragAndDropComponentSignalMapper, SIGNAL(mapped(QString)),this,SLOT(makeComponentDrag(QString)));

    dragAndDropBusSignalMapper = new QSignalMapper(this);
    connect(dragAndDropBusSignalMapper, SIGNAL(mapped(QString)),this,SLOT(makeBusDrag(QString)));




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
//                    QPointF p = me->scenePos();
//                    Connection *co = getConnectionsFromScene()[0];

                    if (item == 0)
                    {
                        //item = scene->itemAt(me->scenePos(),new QTransform());
                        return false;
                    }

                    QPointF pointF = me->scenePos();
                    QPoint *point = new QPoint();
                    point->setX((int)pointF.x());
                    point->setY((int)pointF.y());


                    Connection *selectedConnection = dynamic_cast<Connection*>(item);
                    if(selectedConnection || item->type() == ComponentView::Type || item->type() == RegularBusView::Type || item->type() == PinView::Type)
                    {
                        activeItem = item;
                        QContextMenuEvent *ev = new QContextMenuEvent(QContextMenuEvent::Mouse, *point,QCursor::pos());
                        QApplication::postEvent(this,ev);
                    }
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
            if (!(conn && me->button() == Qt::LeftButton))
                break;

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
                            QString instanceName = ui->graphicsView->createUniqueInstanceName(a->id(), num);
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
    }
    return QObject::eventFilter(o, e);
}
void SchemeEditorMainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    if(event->reason() != QContextMenuEvent::Mouse)
        return;

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
    }

    // Get regular bus via selected bus pin
    PinView *busPin = dynamic_cast<PinView*>(activeItem);
    if(busPin && busPin->parentBus())
    {
        QAction *actionDeleteItem = new QAction(tr("&Delete"), this);
        deleteItemSignalMapper->setMapping(actionDeleteItem, busPin->parentInstanceName());
        connect(actionDeleteItem, SIGNAL(triggered()), deleteItemSignalMapper, SLOT(map()));


        QMenu *contextMenu = new QMenu(this);
        contextMenu->addAction(actionDeleteItem);
        contextMenu->exec(event->globalPos());
    }

    Connection *connection = dynamic_cast<Connection*>(activeItem);
    if(connection)
    {
        QAction *actionDeleteConnection = new QAction(tr("&Delete"), this);
        connect(actionDeleteConnection, SIGNAL(triggered()), this, SLOT(deleteConnection()));

        QMenu *contextMenu = new QMenu(this);
        contextMenu->addAction(actionDeleteConnection);
        contextMenu->exec(event->globalPos());
    }
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
    RegularBusView *bus = getRegularBusFromScene(instanceName);
    if(bus)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Delete item");
        msgBox.setText("Are you sure you want to delete this item?");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        int ret = msgBox.exec();
        if(ret == QMessageBox::Ok)
        {
            QString name = bus->instanceName();
            removeItemFromScene(name);
            scene->update();
            ui->graphicsView->update();
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
            ui->graphicsView->showEditAttribute(attr);
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
        PinView *pb = dynamic_cast<PinView*>(scene->items()[i]);
        if(pb && pb->parentBus() && pb->parentInstanceName() == name)
        {
            RegularBusView *b = pb->parentBus();
            deleteConnections(b);
            scene->removeItem(b);
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
RegularBusView* SchemeEditorMainWindow::getRegularBusFromScene(QString instanceName)
{
    QList<QGraphicsItem*> items = scene->items();

    for(int i = 0; i < items.length(); i++)
    {
        RegularBusView *b = dynamic_cast<RegularBusView*>(items[i]);
        if(b && b->instanceName() == instanceName)
            return b;
    }
    return 0;
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
    RegularBusView *bus = dynamic_cast<RegularBusView*>(parent);
    foreach (QGraphicsItem *item, scene->items())
    {
        if(comp)
        {
            Connection *c = dynamic_cast<Connection*>(item);
            if(c && isConnectionFromPin(c, comp->instanceName()))
                delete item;

        }
        if(bus)
        {
            Connection *c = dynamic_cast<Connection*>(item);
            if(c && isConnectionFromPin(c, bus->instanceName()))
                delete item;

        }
    }
}

bool SchemeEditorMainWindow::isConnectionFromPin(Connection *connection, QString parentComponentInstanceName)
{
    QString pin1Name;
    QString pin2Name;

    if(connection && connection->pin1() && connection->pin2() && connection->pin1()->parentComponent() && (connection->pin2()->parentComponent() || connection->pin2()->parentBus()))
    {
        pin1Name = connection->pin1()->parentInstanceName();
        pin2Name = connection->pin2()->parentInstanceName();

        if(pin1Name == parentComponentInstanceName || pin2Name == parentComponentInstanceName)
            return true;
    }
    return false;
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
QVector<ComponentView*> SchemeEditorMainWindow::getComponentsFromScene()
{
    QVector<ComponentView*> components;

    for(int i = 0; i < scene->items().length(); i++)
    {
        ComponentView *c = dynamic_cast<ComponentView*>(scene->items()[i]);
        if(c)
            components.append(c);
    }
    return components;
}
QVector<RegularBusView*> SchemeEditorMainWindow::getRegularBusesFromScene()
{
    QVector<RegularBusView*> buses;

    for(int i = 0; i < scene->items().length(); i++)
    {
        RegularBusView *c = dynamic_cast<RegularBusView*>(scene->items()[i]);
        if(c)
            buses.append(c);
    }
    return buses;
}
void SchemeEditorMainWindow::deleteConnection()
{
    Connection *connection = dynamic_cast<Connection*>(activeItem);
    if(connection)
        delete connection;
}

void SchemeEditorMainWindow::makeComponentDrag(QString id)
{
    QMimeData *mime = new QMimeData;
    mime->setText(id);
    ComponentModel *c = library->getComponentById(id);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mime);    
    drag->setPixmap(getIconPixmap(c->iconFile()));
    drag->exec();
}
void SchemeEditorMainWindow::makeBusDrag(QString id)
{
    QMimeData *mime = new QMimeData;
    mime->setText(id);
    RegularBus *b = (RegularBus*)library->GetBusByUniqueId(id);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mime);
    drag->setPixmap(getIconPixmap(b->iconFile()));
    drag->exec();
}
