#include "busline.h"

BusLine::BusLine(QString id, int size)
{
    this->ID = id;
    this->size = size;

    busType = BusType::None;
    terminateWith = -1;
    ifUnterminated = -1;
}

void BusLine::setType(QString type)
{
    if(type.isNull() || type.isEmpty())
        return;

    if(type == "wired and")
        busType = BusType::WiredAnd;
    else if(type == "wired or")
        busType = BusType::WiredOr;
    else
        return;
}

void BusLine::setTerminateWith(int x)
{
    if(x)
        terminateWith = x;
}

void BusLine::setIfUnterminated(int x)
{
    if(x)
        ifUnterminated = x;
}
