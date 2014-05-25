#include <QString>

#ifndef ADDRESSSPACE_H
#define ADDRESSSPACE_H

class AddressSpace
{
public:
    AddressSpace();
    QString id;
    QString min;
    QString max;

    qlonglong getMin();
    qlonglong getMax();
};

#endif // ADDRESSSPACE_H
