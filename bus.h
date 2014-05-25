#ifndef BUS_H
#define BUS_H
#include "busline.h"

#include <QVector>
class Bus
{
public:
    Bus();
    QVector<BusLine*> busLines;
};

#endif // BUS_H
