#ifndef SCHEMEEDITORMAINWINDOW_H
#define SCHEMEEDITORMAINWINDOW_H

#include "componentview.h"
#include "editorgraphicsview.h"
#include "libraryfile.h"

#include <QGraphicsView>
#include <QMainWindow>
#include <QDockWidget>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSignalMapper>

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
    void AddComponentToScene(int id);

signals:
    void clicked(int id);

private:
    void createActions();
    bool isComponentInScene(int uid);

    QSignalMapper *signalMapper;
    LibraryFile *library;

    Ui::SchemeEditorMainWindow *ui;
    QGraphicsScene *scene;
};

#endif // SCHEMEEDITORMAINWINDOW_H
