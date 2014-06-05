#ifndef BUS_H
#define BUS_H
#include "busline.h"

#include <QVector>
class Bus
{
public:
    Bus();
    QString ID;
    QVector<BusLine*> busLines;
    int uid;
};

#endif // BUS_H
