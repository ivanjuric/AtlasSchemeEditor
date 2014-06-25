#include "regularbusconnectionrule.h"

RegularBusConnectionRule::RegularBusConnectionRule()
{
}
void RegularBusConnectionRule::addPinIdToInstantiationList(QString pinId)
{
    m_pinInstantiationList.append(pinId);
}
void RegularBusConnectionRule::addToConnectionTerminates(QString pinId)
{
    m_connectionTerminates.append(pinId);
}
void RegularBusConnectionRule::addPopup(RulePopup *popup)
{
    m_popups.append(popup);
}

