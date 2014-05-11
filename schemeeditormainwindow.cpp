#include "schemeeditormainwindow.h"
#include "ui_schemeeditormainwindow.h"
#include "libraryfile.h"

#include <QFileDialog>
#include <QToolButton>

SchemeEditorMainWindow::SchemeEditorMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SchemeEditorMainWindow)
{
    ui->setupUi(this);



    createActions();

    /// Create new Scene add initial components to it
    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);

    QString filePath = "C:\\Users\\Ivan\\Desktop\\FRISC_LIBRARY_SIMPLE.json";

    library = new LibraryFile(filePath);

    signalMapper = new QSignalMapper(this);

    // Fill toolbar with components actions
    foreach(ComponentModel *component, library->componentList){

        QAction *actionAddFromToolbar = new QAction(component->id,this);
        connect(actionAddFromToolbar, SIGNAL(triggered()), signalMapper, SLOT(map()));

        signalMapper->setMapping(actionAddFromToolbar,component->uid);

        ui->toolBar->addAction(actionAddFromToolbar);
    }
    //connect(signalMapper, SIGNAL(mapped(int)),this,SIGNAL(clicked(int)));
    connect(signalMapper, SIGNAL(mapped(int)),this,SLOT(AddComponentToScene(int)));


}
void SchemeEditorMainWindow::selectLibrary()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open config file"),"",tr("Config JSON file (*.json)"));

    LibraryFile* library = new LibraryFile(fileName);

    this->setWindowTitle(library->libraryName);
}

// Podesavanje menija i toolbara
void SchemeEditorMainWindow::createActions(){

//    ui->actionSelectLibrary->setIcon(QIcon(":images/open-file-icon.png"));
//    ui->actionSelectLibrary->setStatusTip(tr("Select library file"));
//    ui->actionSelectLibrary->setToolTip("Select library file");
    connect(ui->actionSelectLibrary, SIGNAL(triggered()), this, SLOT(selectLibrary()));
    ui->mainToolBar->addAction(ui->actionSelectLibrary);


}
// Add selected component to graphics scene
void SchemeEditorMainWindow::AddComponentToScene(int id){
    ComponentModel *component = library->GetComponentByUniqueId(id);

    if(component == 0 || isComponentInScene(component->uid))
        return;


    // Create new component and rewrite importnant properties
    ComponentView *c = new ComponentView(0,0, component);

    scene->addItem(c);


}

// Check if component is already attached to graphics scene
bool SchemeEditorMainWindow::isComponentInScene(int uid)
{
    QList<QGraphicsItem*> items = scene->items();
    for(int i = 0; i < items.length(); i++){
        //Component *c = dynamic_cast<Component*>(items[i]);
        ComponentView *c = (ComponentView*)items[i];
        if(c){
            if(c->model->uid == uid)
                return true;
        }
    }
    return false;
}

SchemeEditorMainWindow::~SchemeEditorMainWindow()
{
    delete ui;
}
