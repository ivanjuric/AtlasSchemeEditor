#include "componentview.h"
#include "regularbusview.h"
#include "libraryfile.h"
#include <QGraphicsView>

#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#pragma once

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(QWidget* parent = NULL);
    void createActions();
    ComponentView* getComponentFromScene(QString name);

    ComponentView* component() { return m_component; }
    RegularBusView* bus() { return m_bus; }
    void setComponent(ComponentView *component) { m_component = component; }
    void setBus(RegularBusView *bus) { m_bus = bus; }

private:

    LibraryFile *m_library;

    ComponentView *m_component;
    RegularBusView *m_bus;



protected:
    //Take over the interaction
    virtual void wheelEvent(QWheelEvent* event);
    double currentScale;


    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
};

#endif // GRAPHICSVIEW_H
