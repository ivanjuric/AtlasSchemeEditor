#ifndef SCHEMEEDITORMAINWINDOW_H
#define SCHEMEEDITORMAINWINDOW_H

#include <QGraphicsView>
#include <QMainWindow>
#include <QDockWidget>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

namespace Ui {
class SchemeEditorMainWindow;
}

class SchemeEditorMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SchemeEditorMainWindow(QWidget *parent = 0);
    ~SchemeEditorMainWindow();


public slots:
    void selectLibrary();

private:
    void createActions();
    Ui::SchemeEditorMainWindow *ui;
    QGraphicsView *view;
    QGraphicsScene *scene;
};

#endif // SCHEMEEDITORMAINWINDOW_H
