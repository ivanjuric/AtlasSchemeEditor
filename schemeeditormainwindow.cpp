#include "schemeeditormainwindow.h"
#include "ui_schemeeditormainwindow.h"
#include "libraryfile.h"

#include <QFileDialog>

SchemeEditorMainWindow::SchemeEditorMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SchemeEditorMainWindow)
{
    ui->setupUi(this);

    createActions();

    /// Create new Scene add initial components to it
    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);

    QString filePath = "C:\\Users\\Ivan\\Desktop\\frisc.json";

    LibraryFile* library = new LibraryFile(filePath);
}
void SchemeEditorMainWindow::selectLibrary()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open config file"),"",tr("Config JSON file (*.json)"));

    LibraryFile* library = new LibraryFile(fileName);

    this->setWindowTitle(library->libraryName);
}

// Podesavanje menija i toolbara
void SchemeEditorMainWindow::createActions(){

    //actionSelectLibrary->setIcon(QIcon(":/images/pdf.png"));
    //ui->actionSelectLibrary->setStatusTip(tr("Select library file"));
    //ui->actionSelectLibrary->setToolTip("Select library file");
    connect(ui->actionSelectLibrary, SIGNAL(triggered()), this, SLOT(selectLibrary()));
    //ui->mainToolBar->addAction(ui->actionSelectLibrary);

}

SchemeEditorMainWindow::~SchemeEditorMainWindow()
{
    delete ui;
}
