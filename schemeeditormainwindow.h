#ifndef SCHEMEEDITORMAINWINDOW_H
#define SCHEMEEDITORMAINWINDOW_H

#include "componentview.h"
#include "editorgraphicsview.h"
#include "libraryfile.h"
#include "connection.h"

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


public slots:
    void selectLibrary();
    void saveSceneToFile();
    void loadSceneFromFile();
    void AddComponentToScene(int id);
    void AddBusToScene(int id);

signals:
    void clicked(int id);

private:
    void createActions();
    bool isComponentInScene(int uid);
    bool isBusInScene(int uid);
    QGraphicsItem *itemAt(const QPointF&);

    QSignalMapper *signalMapper;

    LibraryFile *library;

    Ui::SchemeEditorMainWindow *ui;
    QGraphicsScene *scene;

    Connection *conn;
    void deleteItem(QGraphicsItem *item);
};

#endif // SCHEMEEDITORMAINWINDOW_H
