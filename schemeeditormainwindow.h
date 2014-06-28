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
    int getNumberOfSameComponentsInScene(QString baseName);
    ComponentView *getComponentFromScene(QString instanceName);
    int findComponentNameIndex(QString baseName);


public slots:
    void selectLibrary();
    void saveSceneToFile();
    void loadSceneFromFile();
    void AddComponentToScene(QString id);
    void AddBusToScene(QString id);
    void mirror(QString instanceName);
    void deleteItem(QString instanceName);
    void editAttributes(QString instanceName);
    void clearScene();
    
    void createComponentContextMenu(QString instanceName);


signals:
    void clicked(int id);

private:
    void createActions();
    bool isComponentInScene(int uid);
    bool isSameBusInScene(QString id);
    QGraphicsItem *itemAt(const QPointF&);

    QSignalMapper *componentsSignalMapper;
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

    QString createUniqueInstanceName(QString name, int num);

    bool isConnectionAllowed(PinView *pin1, PinView *pin2);
    bool checkRegularBusRule(PinView *pin1, PinView *pin2);
    QString checkAutomaticBusRule(PinView *pin1, PinView *pin2);

    int getNumberOfSameBusesInScene(QString id);
    int findBusNameIndex(QString id);

    QVector<Connection*> getConnectionsFromScene();
    QVector<AutomaticBus*> getAutomaticBusesFromScene();
    QVector<ComponentView*> getComponentsFromScene();
    QVector<RegularBusView*> getRegularBusesFromScene();


    QPixmap getIconPixmap(QString path);

    void showEditAttribute(Attribute *attribute);

    ComponentView* createComponentViewFromFile(QString id, bool mirrored, QString instanceName, QPointF pos, QMap<QString,int> attributes);
    RegularBusView* createRegularBusViewFromFile(QString id, QPointF pos, QString instanceName);
    Connection* createConnectionFromFile(QString parentName1,QString pin1,QString parentName2,QString pin2,AutomaticBus *automaticBus);

protected:
    void contextMenuEvent(QContextMenuEvent *event);

//    virtual void dragEnterEvent(QDragEnterEvent *event);
//    virtual void dragLeaveEvent(QDragLeaveEvent *event);
//    virtual void dragMoveEvent(QDragMoveEvent *event);
//    virtual void dropEvent(QDropEvent *event);
};

#endif // SCHEMEEDITORMAINWINDOW_H
