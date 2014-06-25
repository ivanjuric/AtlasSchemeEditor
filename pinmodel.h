#include <QtGui>
#include "Enums.h"

#ifndef PINMODEL_H
#define PINMODEL_H

#pragma once

class PinModel
{
public:
    // Constructor
    PinModel(int uid);

    // Getters
    int uid() { return m_uid; }
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

    // Setters
    void setUid(int uid) { m_uid = uid; }
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


private:
    int m_uid;
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
};

#endif // PINMODEL_H
