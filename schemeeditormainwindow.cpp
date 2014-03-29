#include "schemeeditormainwindow.h"
#include "ui_schemeeditormainwindow.h"
#include <QDockWidget>

SchemeEditorMainWindow::SchemeEditorMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SchemeEditorMainWindow)
{
    ui->setupUi(this);

    /// Create new Scene add initial components to it
    scene = new QGraphicsScene();



    ui->graphicsView->setScene(scene);

    //SchemeEditor *editor = new SchemeEditor();
    //editor->ParseJson();
}

SchemeEditorMainWindow::~SchemeEditorMainWindow()
{
    delete ui;
}
