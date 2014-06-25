#include "busline.h"
#include <QVector>

#ifndef BUS_H
#define BUS_H

#pragma once


class Bus
{
public:
    // Constructor
    Bus();

    // Getters
    QString id() { return m_id; }
    QVector<BusLine*> busLines() { return m_busLines; }

    // Setters
    void setId(QString id) { m_id = id; }

    // Methods
    void addBusLine(BusLine *busLine);

private:
    QString m_id;
    QVector<BusLine*> m_busLines;
};

#endif // BUS_H
