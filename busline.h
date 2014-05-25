#include <QString>

#ifndef BUSLINE_H
#define BUSLINE_H

class BusLine
{
public:
    BusLine();
    QString ID;
    int size;
    enum Type { WiredAnd };
    int terminateWith;
};

#endif // BUSLINE_H
