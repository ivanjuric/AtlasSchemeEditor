#include "qstring.h"

#ifndef BUSLINE_H
#define BUSLINE_H

#pragma once


class BusLine
{
public:
    BusLine(QString id, int size);
    QString ID;
    int size;
    enum BusType { None = 0, WiredAnd = 1, WiredOr = 2 };
    BusType busType;
    int terminateWith;
    int ifUnterminated;

    void setType(QString type);
    void setTerminateWith(int x);
    void setIfUnterminated(int x);
};

#endif // BUSLINE_H
