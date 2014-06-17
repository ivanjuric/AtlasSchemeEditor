#include "schemeeditormainwindow.h"
#include "ui_schemeeditormainwindow.h"
#include "libraryfile.h"
#include "regularbusview.h"

#include <QFileDialog>
#include <QToolButton>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QWheelEvent>
#include <QContextMenuEvent>
#include <QGraphicsItem>

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
void SchemeEditorMainWindow::fillToolbar()
{
    foreach (ComponentModel *c, library->componentList)
    {
        QAction *actionAddFromToolbar = new QAction(c->id(),this);
        componentsSignalMapper->setMapping(actionAddFromToolbar,c->id());
        connect(actionAddFromToolbar, SIGNAL(triggered()), componentsSignalMapper, SLOT(map()));
        ui->toolBar->addAction(actionAddFromToolbar);
    }

    ui->toolBar->addSeparator();

    foreach(Bus *bus, library->regularBuses)
    {
        QAction *actionAddFromToolbar = new QAction(bus->ID,this);
        signalMapper->setMapping(actionAddFromToolbar,bus->uid);
        connect(actionAddFromToolbar, SIGNAL(triggered()), signalMapper, SLOT(map()));
        ui->toolBar->addAction(actionAddFromToolbar);
    }
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

    signalMapper = new QSignalMapper(this);
    connect(signalMapper, SIGNAL(mapped(int)),this,SLOT(AddBusToScene(int)));


//    ui->actionSelectLibrary->setIcon(QIcon(":images/open-file-icon.png"));
//    ui->actionSelectLibrary->setStatusTip(tr("Select library file"));
//    ui->actionSelectLibrary->setToolTip("Select library file");
    connect(ui->actionSelectLibrary, SIGNAL(triggered()), this, SLOT(selectLibrary()));
    ui->mainToolBar->addAction(ui->actionSelectLibrary);

    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveSceneToFile()));
    ui->mainToolBar->addAction(ui->actionSave);


    connect(ui->actionLoad, SIGNAL(triggered()), this, SLOT(loadSceneFromFile()));
    ui->mainToolBar->addAction(ui->actionLoad);



    connect(ui->actionClearScene, SIGNAL(triggered()), this, SLOT(clearScene()));
    ui->mainToolBar->addAction(ui->actionClearScene);

}
// Add selected component to graphics scene
void SchemeEditorMainWindow::AddComponentToScene(QString id)
{
    ComponentModel *model = library->getComponentById(id);
    //int numberOfSameComponents = getNumberOfSameComponentsInScene(component->instanceNameBase());

    if(model == 0)
        return;

    int numberOfSameComponents = getNumberOfSameComponentsInScene(model->instanceNameBase());
    if(model->maxInstances() != 0 && numberOfSameComponents >= model->maxInstances())
        return;

    // Create new component and rewrite importnant properties
    ComponentView *c = new ComponentView(model, QPoint(0,0));
    int instanceNameIndex = findComponentNameIndex(c->instanceNameBase());
    c->setUniqueInstanceName(instanceNameIndex);
    c->setParentToPins();
    foreach(PinView *pin, c->pins())
    {
        pin->setStartPosition();
        pin->setFlag(QGraphicsItem::ItemIsMovable,false);
        pin->setParentItem(c);
    }
    c->setMirrored(false);
    scene->addItem(c);
}

// Add selected bus to graphics scene
void SchemeEditorMainWindow::AddBusToScene(int id)
{
    Bus *bus = library->GetBusByUniqueId(id);


    if(bus == 0 || isBusInScene(bus->uid))
        return;


    // Create new component and rewrite importnant properties
    RegularBusView *b = new RegularBusView(0,0,(RegularBus*)bus);

    scene->addItem(b);

    foreach(PinView *pin, b->busPins)
    {
        pin->setStartPosition();
        pin->setFlag(QGraphicsItem::ItemIsMovable,false);
        pin->setParentItem(b);
    }
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
bool SchemeEditorMainWindow::isBusInScene(int uid)
{
    QList<QGraphicsItem*> items = scene->items();
    for(int i = 0; i < items.length(); i++){
        RegularBusView *b = dynamic_cast<RegularBusView*>(items[i]);
        //ComponentView *c = (ComponentView*)items[i];
        if(b && b->model && b->model->uid && b->model->uid == uid)
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

                    if (item && item->type() == PinView::Type)
                    {
                        QGraphicsItem *parent = item->parentItem();
                        if(parent && parent->type() == RegularBusView::Type)
                            break;

                        conn = new Connection(0);
                        scene->addItem(conn);
                        PinView *pin1 = (PinView*)item;
                        conn->setPin1(pin1);
                        QPointF startP = pin1->scenePos();
                        startP.setX(startP.x() + pin1->width()/2);
                        startP.setY(startP.y() + pin1->height()/2);
                        conn->setPos1(startP);
                        conn->setPos2(me->scenePos());
                        conn->updatePath();

                        return true;
                    }
                    else if (item && item->type() == ComponentView::Type)
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
                    QPointF pointF = me->scenePos();
                    QPoint *point = new QPoint();
                    point->setX((int)pointF.x());
                    point->setY((int)pointF.y());
                    QGraphicsItem *item = itemAt(me->scenePos());

                    if (item)
                    {
                        if((item->type() == Connection::Type || item->type() == ComponentView::Type) || item->type() == RegularBusView::Type)
                        {
                            activeItem = item;
                            //deleteItem(item);
                            QContextMenuEvent *ev = new QContextMenuEvent(QContextMenuEvent::Mouse, *point,QCursor::pos());
                            QApplication::postEvent(this,ev);
                        }

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

                    if (pin1->parentComponent() != pin2->parentComponent()  && !pin1->isConnected(pin2))
                    {
                        conn->setPos2(pin2->centerPos((PinView*)item));
                        conn->setPin2(pin2);
                        conn->updatePath();
                        conn = 0;
                        return true;
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
void SchemeEditorMainWindow::deleteItem(QGraphicsItem *item)
{

}

void SchemeEditorMainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    if(event->reason() == QContextMenuEvent::Mouse)
    {
        ComponentView *component = dynamic_cast<ComponentView*>(activeItem);
        if(component)
        {
            QAction *actionMirror = new QAction(tr("&Mirror"), this);
            actionMirror->setStatusTip(tr("Mirror component pins"));
            // Map components actions to context menu
            contextMenuSignalMapper->setMapping(actionMirror,component->instanceName());
            connect(actionMirror, SIGNAL(triggered()), contextMenuSignalMapper, SLOT(map()));

            QMenu menu(this);
            menu.addAction(actionMirror);
            menu.exec(event->globalPos());
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
        //ui->graphicsView->viewport()->update();
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

int SchemeEditorMainWindow::findComponentNameIndex(QString baseName)
{
    QList<QGraphicsItem*> items = scene->items();

    int max = 0;
    for(int i = 0; i < items.length(); i++)
    {
        ComponentView *c = dynamic_cast<ComponentView*>(items[i]);
        //ComponentView *c = (ComponentView*)items[i];
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

void SchemeEditorMainWindow::updateConnections()
{
    foreach (QGraphicsItem *item, scene->items())
    {
        Connection *c = dynamic_cast<Connection*>(item);
        if(c)
            c->updatePath();
    }
}
