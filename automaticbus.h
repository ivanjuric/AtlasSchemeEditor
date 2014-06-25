#include "bus.h"

#ifndef AUTOMATICBUS_H
#define AUTOMATICBUS_H

#pragma once

class AutomaticBus : public Bus
{
public:
    AutomaticBus();

    QString instanceName() { m_instanceName; }
    void setInstanceName(QString instanceName) { m_instanceName = instanceName; }

private:
    QString m_instanceName;
};

#endif // AUTOMATICBUS_H
