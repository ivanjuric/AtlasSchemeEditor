#include "schemeeditormainwindow.h"
#include "ui_schemeeditormainwindow.h"
#include "libraryfile.h"
#include "busview.h"

#include <QFileDialog>
#include <QToolButton>
#include <QGraphicsSceneMouseEvent>

SchemeEditorMainWindow::SchemeEditorMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SchemeEditorMainWindow)
{
    conn = 0;
    ui->setupUi(this);

    createActions();

    /// Create new Scene add initial components to it
    scene = new QGraphicsScene();
    scene->installEventFilter(this);
    ui->graphicsView->setScene(scene);

    QString filePath = "C:\\Users\\Ivan\\Desktop\\FRISC_LIBRARY_SIMPLE.json";

    library = new LibraryFile(filePath);

    signalMapper = new QSignalMapper(this);

    // Fill toolbar with components actions
    foreach(ComponentModel *component, library->componentList)
    {
        QAction *actionAddFromToolbar = new QAction(component->id,this);
        connect(actionAddFromToolbar, SIGNAL(triggered()), signalMapper, SLOT(map()));

        signalMapper->setMapping(actionAddFromToolbar,component->uid);

        ui->toolBar->addAction(actionAddFromToolbar);
    }
    //connect(signalMapper, SIGNAL(mapped(int)),this,SIGNAL(clicked(int)));
    connect(signalMapper, SIGNAL(mapped(int)),this,SLOT(AddComponentToScene(int)));

    ui->toolBar->addSeparator();

    foreach(Bus *bus, library->regularBuses)
    {
        QAction *actionAddFromToolbar = new QAction(bus->ID,this);
        connect(actionAddFromToolbar, SIGNAL(triggered()), signalMapper, SLOT(map()));

        signalMapper->setMapping(actionAddFromToolbar,bus->uid);

        ui->toolBar->addAction(actionAddFromToolbar);
    }
    connect(signalMapper, SIGNAL(mapped(int)),this,SLOT(AddBusToScene(int)));

}
void SchemeEditorMainWindow::selectLibrary()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open config file"),"",tr("Config JSON file (*.json)"));

    LibraryFile* library = new LibraryFile(fileName);

    this->setWindowTitle(library->libraryTitle);
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
    ds << library->filePath;
    // Save all components and its pin children
    foreach(QGraphicsItem *item, scene->items())
    {
        if (item->type() == ComponentView::Type)
        {
            ds << item->type();
            ((ComponentView*) item)->save(ds);
        }
    }
}
void SchemeEditorMainWindow::load(QDataStream &ds)
{
    scene->clear();
    QMap<quint64, PinView*> pinMap;

    QString libraryFilePath;
    ds >> libraryFilePath;

    library = new LibraryFile(libraryFilePath);

    while (!ds.atEnd())
    {
        int type;
        ds >> type;
        if (type == ComponentView::Type)
        {
            ComponentView *component = new ComponentView();
            scene->addItem(component);
            component->load(ds, pinMap);
        }
    }
}

// Podesavanje menija i toolbara
void SchemeEditorMainWindow::createActions()
{

//    ui->actionSelectLibrary->setIcon(QIcon(":images/open-file-icon.png"));
//    ui->actionSelectLibrary->setStatusTip(tr("Select library file"));
//    ui->actionSelectLibrary->setToolTip("Select library file");
    connect(ui->actionSelectLibrary, SIGNAL(triggered()), this, SLOT(selectLibrary()));
    ui->mainToolBar->addAction(ui->actionSelectLibrary);

    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveSceneToFile()));
    ui->mainToolBar->addAction(ui->actionSave);


    connect(ui->actionLoad, SIGNAL(triggered()), this, SLOT(loadSceneFromFile()));
    ui->mainToolBar->addAction(ui->actionLoad);

}
// Add selected component to graphics scene
void SchemeEditorMainWindow::AddComponentToScene(int id){
    ComponentModel *component = library->GetComponentByUniqueId(id);

    if(component == 0 || isComponentInScene(component->uid))
        return;


    // Create new component and rewrite importnant properties
    ComponentView *c = new ComponentView(0,0, component);

    scene->addItem(c);

    foreach(PinView *pin, c->model->visualPins)
    {
        //QPointF *pos = pin->getStartPosition();
        //pin->setPos(*pos);
        pin->setStartPosition();
        pin->setFlag(QGraphicsItem::ItemIsMovable,false);
        pin->setParentItem(c);
    }
}

// Add selected bus to graphics scene
void SchemeEditorMainWindow::AddBusToScene(int id)
{
    Bus *bus = library->GetBusByUniqueId(id);

    if(bus == 0 || isBusInScene(bus->uid))
        return;


    // Create new component and rewrite importnant properties
    BusView *b = new BusView(0,0,(RegularBus*)bus);

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
    QList<QGraphicsItem*> items = scene->items();
    for(int i = 0; i < items.length(); i++){
        ComponentView *c = dynamic_cast<ComponentView*>(items[i]);
        //ComponentView *c = (ComponentView*)items[i];
        if(c && c->model && c->model->uid && c->model->uid == uid)
            return true;
    }
    return false;
}
// Check if component is already attached to graphics scene
bool SchemeEditorMainWindow::isBusInScene(int uid)
{
    QList<QGraphicsItem*> items = scene->items();
    for(int i = 0; i < items.length(); i++){
        BusView *b = dynamic_cast<BusView*>(items[i]);
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
                        conn = new Connection(0);
                        scene->addItem(conn);
                        PinView *pin1 = (PinView*)item;
                        conn->setPin1(pin1);
                        QPointF startP = pin1->scenePos();
                        startP.setX(startP.x() + pin1->width/2);
                        startP.setY(startP.y() + pin1->height/2);
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
                    QGraphicsItem *item = itemAt(me->scenePos());
                    if (item && (item->type() == Connection::Type || item->type() == ComponentView::Type))
                        deleteItem(item);
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

                    if (pin1->component() != pin2->component()  && !pin1->isConnected(pin2))
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
