#include "pinrulepopup.h"

PinRulePopup::PinRulePopup()
{
}
void PinRulePopup::addBusLineToList(QString busLineId)
{
    if(!busLineList().contains(busLineId))
        m_busLineList.append(busLineId);
}
