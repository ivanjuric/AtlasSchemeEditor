#include "qgraphicsitem.h"
#include "Enums.h"
#include "pinmodel.h"

#ifndef PINVIEW_H
#define PINVIEW_H

#pragma once


class Connection;
class ComponentView;
class RegularBusView;

class PinView : public QGraphicsItem
{
public:
    PinView();
    PinView(PinModel *model);

    // Getters
    QString id() { return m_id; }
    QString title() { return m_title; }
    QString tooltip() { return m_tooltip; }
    PinTypeEnum shape() { return m_shape; }
    int x() { return m_x; }
    int y() { return m_y; }
    int width() { return m_width; }
    int height() { return m_height; }
    PinSideEnum side() { return m_side; }
    QColor lineColor() { return m_lineColor; }
    QColor fillColor() { return m_fillColor; }
    QColor lineColorConnected() { return m_lineColorConnected; }
    QColor fillColorConnected() { return m_fillColorConnected; }

    ComponentView *parentComponent() { return m_parentComponent; }
    RegularBusView *parentBus() { return m_parentBus; }
    QString parentInstanceName() { return m_parentInstanceName; }

    // Setters
    void setId(QString id) { m_id = id; }
    void setTitle(QString title) { m_title = title; }
    void setTooltip(QString tooltip) { m_tooltip = tooltip; }
    void setShape(PinTypeEnum shape) { m_shape = shape; }
    void setX(int x) { m_x = x; }
    void setY(int y) { m_y = y; }
    void setWidth(int width) { m_width = width; }
    void setHeight(int height) { m_height = height; }
    void setSide(PinSideEnum side) { m_side = side; }
    void setLineColor(QColor lineColor) { m_lineColor = lineColor; }
    void setFillColor(QColor fillColor) { m_fillColor = fillColor; }
    void setLineColorConnected(QColor lineColorConnected) { m_lineColorConnected = lineColorConnected; }
    void setFillColorConnected(QColor fillColorConnected) { m_fillColorConnected = fillColorConnected; }

    void setParentComponent(ComponentView *parentComponent);
    void setParentBus(RegularBusView *parentBus);
    void setParentInstanceName(QString name) {m_parentInstanceName = name;}

private:
    // Properties from model
    QString m_id;
    QString m_title;
    QString m_tooltip;
    PinTypeEnum m_shape;
    int m_x;
    int m_y;
    int m_width;
    int m_height;
    PinSideEnum m_side;
    QColor m_lineColor;
    QColor m_fillColor;
    QColor m_lineColorConnected;
    QColor m_fillColorConnected;
    QString m_checkConnection;

    ComponentView *m_parentComponent;
    RegularBusView *m_parentBus;

    QString m_parentInstanceName;






public:

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);

    void drawPin(QPainter *painter, PinTypeEnum type);
    void drawBusPin(QPainter *painter);
    void drawSquare(QPainter *painter);
    void drawCircle(QPainter *painter);
    void drawInOut(QPainter *painter);
    void drawRL(QPainter *painter);
    void drawLR(QPainter *painter);
    void drawTriangle(QPainter *painter, QPoint a, QPoint b, QPoint c);

    QVector<Connection*> connections;

    enum { Type = QGraphicsItem::UserType + 1 };
    enum { NamePort = 1, TypePort = 2 };
    int type() const { return Type; }
    bool isConnected();
    void setComponent(ComponentView *c);
    void setSide(QString side);

    QPointF *getStartPosition();
    void setStartPosition();
    QPointF centerPos(PinView *pin);

    QGraphicsTextItem *label;
    void setLabel();

    void setValuesFromLibrary(PinModel *model);
    void switchSides();


protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    void copyPin(PinView *pin);
    int margin;
    void updatePositionAfterMirror();
    void updateLabelPosition();

};

#endif // PINVIEW_H
