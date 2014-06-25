#pragma once
#include "Enums.h"
#include "bus.h"
#include <QtGui>

#ifndef REGULARBUS_H
#define REGULARBUS_H

#pragma once



class RegularBus : public Bus
{
public:
    // Constructor
    RegularBus();

    // Getters
    QString iconFile() { return m_iconFile; }
    QString title() { return m_title; }
    QString tooltip() { return m_tooltip; }
    int minInstances() { return m_minInstances; }
    int maxInstances() { return m_maxInstances; }
    OrientationEnum orientation() { return m_orientation; }
    int lineLength() { return m_lineLength; }
    int lineThickness() { return m_lineThickness; }
    QColor lineColor() { return m_lineColor; }

    // Setters
    void setIconFile(QString iconFile) { m_iconFile = iconFile; }
    void setTitle(QString title) { m_title = title; }
    void setTooltip(QString tooltip) { m_tooltip = tooltip; }
    void setMinInstances(int minInstances) { m_minInstances = minInstances; }
    void setMaxInstances(int maxInstances) { m_maxInstances = maxInstances; }
    void setOrientation(OrientationEnum orientation) { m_orientation = orientation; }
    void setLineLength(int lineLength) { m_lineLength = lineLength; }
    void setLineThickness(int lineThickness) { m_lineThickness = lineThickness; }
    void setLineColor(QColor lineColor) { m_lineColor = lineColor; }

private:
    QString m_iconFile;
    QString m_title;
    QString m_tooltip;
    int m_minInstances;
    int m_maxInstances;

    OrientationEnum m_orientation;
    int m_lineLength;
    int m_lineThickness;
    QColor m_lineColor;
};

#endif // REGULARBUS_H
