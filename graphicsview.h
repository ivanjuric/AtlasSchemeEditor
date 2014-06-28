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

    // Getters
    LibraryFile* library() { return m_library; }
    RegularBusView* bus() { return m_bus; }
    double currentScale() { return m_currentScale; }

    // Setters
    void setLibrary(LibraryFile* library) { m_library = library; }
    void setBus(RegularBusView *bus) { m_bus = bus; }
    void setCurrentScale(double currentScale) { m_currentScale = currentScale; }

    // Public methods
    void createActions();

    // Component helper methods
    ComponentView* getComponentFromScene(QString name);
    ComponentView* createComponent(QString id, QPointF pos);
    int getNumberOfSameComponentsInScene(QString baseName);
    int findComponentNameIndex(QString baseName);
    void showEditAttribute(Attribute *attribute);


    // Bus helper methods
    RegularBusView *createBus(QString id, QPointF pos);
    bool isSameBusInScene(QString id);
    int findBusNameIndex(QString id);
    int getNumberOfSameBusesInScene(QString id);

    // General helper methods
    QString createUniqueInstanceName(QString name, int num = 0);

private:
    // Members
    LibraryFile *m_library;
    RegularBusView *m_bus;
    double m_currentScale;

    // Private methods

    // Inherited from base class

    virtual void wheelEvent(QWheelEvent* event);
    // Drag&Drop
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
};

#endif // GRAPHICSVIEW_H
