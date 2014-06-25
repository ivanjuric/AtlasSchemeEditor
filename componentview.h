#include "componentmodel.h"
#include "visualcircle.h"
#include "visualrectangle.h"
#include "visualtext.h"
#include "pinview.h"

#include <QAction>
#include "qgraphicsitem.h"

#ifndef COMPONENTVIEW_H
#define COMPONENTVIEW_H

#pragma once

class ComponentView : public QGraphicsItem
{
public:
    ComponentView(ComponentModel *model, QPoint pos);

    // Getters
    QString id() { return m_id; }
    QString title() { return m_title; }
    QString instanceNameBase() { return m_instanceNameBase; }
    QString iconFile() { return m_iconFile; }
    QString tooltip() { return m_tooltip; }
    int minInstances() { return m_minInstances; }
    int maxInstances() { return m_maxInstances; }
    QString comdelFile() { return m_comdelFile; }
    QVector<VisualComponentElement*> visualElements() { return m_visualElements; }
    QVector<PinView*> pins() { return m_pins; }
    QVector<Attribute*> attributes() { return m_attributes; }



    QString instanceName() { return m_instanceName; }
    void setInstanceName(QString instanceName) { m_instanceName = instanceName; }
    int x() { return m_x; }
    int y() { return m_y; }
    int width() { return m_width; }
    int height() { return m_height; }

    bool mirrored() { return m_mirrored; }

    enum { Type = QGraphicsItem::UserType + 3 };
    int type() const { return Type; }

    void setParentToPins();
    void setUniqueInstanceName(int num);
    void setMirrored(bool mirrored) { m_mirrored = mirrored; }
    void mirrorSides();


private:
    QString m_id;
    QString m_title;
    QString m_instanceNameBase;
    QString m_iconFile;
    QString m_tooltip;
    int m_minInstances;
    int m_maxInstances;
    QString m_comdelFile;
    QVector<VisualComponentElement*> m_visualElements;
    QVector<PinView*> m_pins;
    QVector<Attribute*> m_attributes;

    // Setters
    void setId(QString id) { m_id = id; }
    void setTitle(QString title) { m_title = title; }
    void setInstanceNameBase(QString instanceNameBase) { m_instanceNameBase = instanceNameBase; }
    void setIconFile(QString iconFile) { m_iconFile = iconFile; }
    void setTooltip(QString tooltip) { m_tooltip = tooltip; }
    void setMinInstances(int minInstances) { m_minInstances = minInstances; }
    void setMaxInstances(int maxInstances) { m_maxInstances = maxInstances; }
    void setComdelFile(QString comdelFile) { m_comdelFile = comdelFile; }


    //Dynamic properties
    QString m_instanceName;
    int m_x;
    int m_y;
    int m_width;
    int m_height;

    bool m_mirrored;

    void setX(int x) { m_x = x; }
    void setY(int y) { m_y = y; }
    void setWidth(int width) { m_width = width; }
    void setHeight(int height) { m_height = height; }


    // Methods
    void setDimensions();


    // Graphic methods
public:
    QRectF boundingRect() const;
private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);
    void drawVisualElement(QPainter *painter, VisualComponentElement *visualElement);
    void drawVisualRectangle(QPainter *painter, VisualRectangle *rect);
    void drawVisualText(QPainter *painter, VisualText *text);
    void drawVisualCircle(QPainter *painter, VisualCircle *circle);







    // -------------------  STARI KOD -----------------


    void save(QDataStream &ds);
    void load(QDataStream &ds, QMap<quint64, PinView*> &pinMap);








};

#endif // COMPONENTVIEW_H
