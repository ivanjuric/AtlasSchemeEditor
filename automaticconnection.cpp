#include "automaticconnection.h"

AutomaticConnection::AutomaticConnection()
{
}
void AutomaticConnection::addToPinInstantiationList(QString pin)
{
    m_pinInstantiationList.append(pin);
}
