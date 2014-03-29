#include "schemeeditormainwindow.h"
#include "ui_schemeeditormainwindow.h"

SchemeEditorMainWindow::SchemeEditorMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SchemeEditorMainWindow)
{
    ui->setupUi(this);
}

SchemeEditorMainWindow::~SchemeEditorMainWindow()
{
    delete ui;
}
