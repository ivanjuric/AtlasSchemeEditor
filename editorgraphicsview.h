#include "componentview.h"

#include <QGraphicsView>

#ifndef EDITORGRAPHICSVIEW_H
#define EDITORGRAPHICSVIEW_H

#pragma once

class EditorGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    EditorGraphicsView(QWidget* parent = NULL);
    void createActions();
    ComponentView* getComponentFromScene(QString name);
protected:
    //Take over the interaction
    virtual void wheelEvent(QWheelEvent* event);
    double currentScale;
//    virtual void enterEvent(QEvent *event);
//    virtual void mousePressEvent(QMouseEvent *event);
//    virtual void mouseReleaseEvent(QMouseEvent *event);
    //void contextMenuEvent(QContextMenuEvent *event);

//public slots:
//    void mirror();

//private:
//    QAction *actionMirror;
//    QGraphicsItem* itemAt(const QPointF &pos);
};

#endif // EDITORGRAPHICSVIEW_H
