#include "regularbus.h"
#include "Enums.h"
#include "pinview.h"
#include "qgraphicsitem.h"
#include "qcolor.h"

#include <QLabel>

#ifndef REGULARBUSVIEW_H
#define REGULARBUSVIEW_H

#pragma once

class PinView;
class RegularBusView : public QGraphicsItem
{
public:
    // Constructor
    RegularBusView(RegularBus *model);

    // Getters
    int x() { return m_x; }
    int y() { return m_y; }
    QVector<PinView*> busPins() { return m_busPins; }

    QString iconFile() { return m_iconFile; }
    QString title() { return m_title; }
    QString tooltip() { return m_tooltip; }
    int minInstances() { return m_minInstances; }
    int maxInstances() { return m_maxInstances; }
    OrientationEnum orientation() { return m_orientation; }
    int lineLength() { return m_lineLength; }
    int lineThickness() { return m_lineThickness; }
    QColor lineColor() { return m_lineColor; }

    QString instanceName() { return m_instanceName; }
    QString id() { return m_id; }
    QVector<BusLine*> busLines() { return m_busLines; }
    QGraphicsTextItem* label() { return m_label; }

    // Setters
    void setX(int x) { m_x = x; }
    void setY(int y) { m_y = y; }
    void setIconFile(QString iconFile) { m_iconFile = iconFile; }
    void setTitle(QString title) { m_title = title; }
    void setTooltip(QString tooltip) { m_tooltip = tooltip; }
    void setMinInstances(int minInstances) { m_minInstances = minInstances; }
    void setMaxInstances(int maxInstances) { m_maxInstances = maxInstances; }
    void setOrientation(OrientationEnum orientation) { m_orientation = orientation; }
    void setLineLength(int lineLength) { m_lineLength = lineLength; }
    void setLineThickness(int lineThickness) { m_lineThickness = lineThickness; }
    void setLineColor(QColor lineColor) { m_lineColor = lineColor; }
    void setInstanceName(QString name) { m_instanceName = name; }

    //void setUid(QString uid) { m_instanceName = uid; }
    void setId(QString id) { m_id = id; }

    void addBusPin(PinView *pin);
    void addBusLine(BusLine *busLine);
    void setUniqueInstanceName(int num);

    enum { Type = QGraphicsItem::UserType + 5 };

    void setLabel();
    QRectF boundingRect() const;

private:
    int m_x;
    int m_y;
    QVector<PinView*> m_busPins;

    QString m_iconFile;
    QString m_title;
    QString m_tooltip;
    int m_minInstances;
    int m_maxInstances;

    OrientationEnum m_orientation;
    int m_lineLength;
    int m_lineThickness;
    QColor m_lineColor;

    QString m_instanceName;
    QString m_id;
    QVector<BusLine*> m_busLines;

    QGraphicsTextItem *m_label;
    void updateLabelPosition();


    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);
    void fillBusPins();
    PinView *createPinForBus(QString id, QColor color, int x, int y, int width, int height);
};
#endif // REGULARBUSVIEW_H
