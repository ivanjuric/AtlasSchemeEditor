#include "componentview.h"
#include "graphicsview.h"
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

#ifndef SCHEMEEDITORMAINWINDOW_H
#define SCHEMEEDITORMAINWINDOW_H

#pragma once


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
    ComponentView *getComponentFromScene(QString instanceName);
    RegularBusView* getRegularBusFromScene(QString instanceName);


public slots:

    void saveSceneToFile();
    void loadSceneFromFile();
    void AddBusToScene(QString id);
    void mirror(QString instanceName);
    void deleteItem(QString instanceName);
    void editAttributes(QString instanceName);
    void clearScene();
    void deleteConnection();
    
    void createComponentContextMenu(QString instanceName);

    void makeComponentDrag(QString id);
    void makeBusDrag(QString id);


signals:
    void clicked(int id);

private:
    void createActions();
    void setEditorWindowTitle(QString libraryName);
    bool isComponentInScene(int uid);
    QGraphicsItem *itemAt(const QPointF&, bool select = false);

    QSignalMapper *busSignalMapper;
    QSignalMapper *contextMenuSignalMapper;

    
    QSignalMapper *componentContextMenuSignalMapper;
    
    // Context menus, actions and signal mappers
    QMenu *componentContextMenu;
    QMenu *busContextMenu;
    QMenu *connectionContextMenu;

    QAction *actionMirrorComponent;
    QAction *actionDeleteItem;

    QSignalMapper *mirrorComponentSignalMapper;
    QSignalMapper *deleteItemSignalMapper;
    QSignalMapper *editAttributesSignalMapper;

    QSignalMapper *dragAndDropComponentSignalMapper;
    QSignalMapper *dragAndDropBusSignalMapper;

    void createContextMenus();

    void removeItemFromScene(QString instanceName);
    void deleteConnections(QGraphicsItem *parent);
    bool isConnectionFromPin(Connection *connection, QString parentComponentInstanceName);

    LibraryFile *library;

    Ui::SchemeEditorMainWindow *ui;
    QGraphicsScene *scene;

    Connection *conn;

//    QList<Connection*> connections;
//    QVector<AutomaticBus*> automaticBuses;

    //QAction *actionMirror;
    void deleteItem(QGraphicsItem *item);
    void fillToolbar();
    void clearSceneView();
    void clearConnections();
    void updateConnections();

    QGraphicsItem *activeItem;
    Connection *activeConnection;


    bool isConnectionAllowed(PinView *pin1, PinView *pin2);
    bool checkRegularBusRule(PinView *pin1, PinView *pin2);
    QString checkAutomaticBusRule(PinView *pin1, PinView *pin2);


    QVector<Connection*> getConnectionsFromScene();
    QVector<AutomaticBus*> getAutomaticBusesFromScene();
    QVector<ComponentView*> getComponentsFromScene();
    QVector<RegularBusView*> getRegularBusesFromScene();


    QPixmap getIconPixmap(QString path);


    ComponentView* createComponentViewFromFile(QString id, bool mirrored, QString instanceName, QPointF pos, QMap<QString,int> attributes);
    RegularBusView* createRegularBusViewFromFile(QString id, QPointF pos, QString instanceName);
    Connection* createConnectionFromFile(QString parentName1,QString pin1,QString parentName2,QString pin2,AutomaticBus *automaticBus);

protected:
    void contextMenuEvent(QContextMenuEvent *event);
private slots:
    void selectLibrary();
    void on_actionSelectLibrary_triggered();
    void on_actionSave_triggered();
    void on_actionLoad_triggered();
    void on_actionClearScene_triggered();
    void on_actionZoomIn_triggered();
    void on_actionZoomOut_triggered();
    void on_actionFitToScene_triggered();
};

#endif // SCHEMEEDITORMAINWINDOW_H
