#include <QtGui>

#ifndef ADDRESSSPACE_H
#define ADDRESSSPACE_H

#pragma once

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
