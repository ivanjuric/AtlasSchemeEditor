#ifndef SCHEMEEDITORMAINWINDOW_H
#define SCHEMEEDITORMAINWINDOW_H

#include "componentview.h"
#include "editorgraphicsview.h"
#include "libraryfile.h"
#include "connection.h"
#include "componentmodel.h"

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

    bool eventFilter(QObject *, QEvent *);

    void save(QDataStream &ds);
    void load(QDataStream &ds);
    int getNumberOfSameComponentsInScene(QString baseName);
    ComponentView *getComponentFromScene(QString instanceName);
    int findComponentNameIndex(QString baseName);


public slots:
    void selectLibrary();
    void saveSceneToFile();
    void loadSceneFromFile();
    void AddComponentToScene(QString id);
    void AddBusToScene(int id);
    void mirror(QString instanceName);
    void clearScene();


signals:
    void clicked(int id);

private:
    void createActions();
    bool isComponentInScene(int uid);
    bool isBusInScene(int uid);
    QGraphicsItem *itemAt(const QPointF&);

    QSignalMapper *componentsSignalMapper;
    QSignalMapper *contextMenuSignalMapper;
    QSignalMapper *signalMapper;

    LibraryFile *library;

    Ui::SchemeEditorMainWindow *ui;
    QGraphicsScene *scene;

    Connection *conn;

    //QAction *actionMirror;
    void deleteItem(QGraphicsItem *item);
    void fillToolbar();
    void clearSceneView();
    void clearConnections();
    void updateConnections();

    QGraphicsItem *activeItem;


protected:
    void contextMenuEvent(QContextMenuEvent *event);
};

#endif // SCHEMEEDITORMAINWINDOW_H
