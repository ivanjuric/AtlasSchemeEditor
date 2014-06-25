#include "automaticbusconnectionrulecomponent.h"

AutomaticBusConnectionRuleComponent::AutomaticBusConnectionRuleComponent()
{
}
void AutomaticBusConnectionRuleComponent::addPinIdToInstantiationList(QString pinId)
{
    m_pinInstantiationList.append(pinId);
}
