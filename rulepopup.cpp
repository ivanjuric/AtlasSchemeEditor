#include "rulepopup.h"

RulePopup::RulePopup()
{
}
void RulePopup::addPinRulePopupToPinList(PinRulePopup *pinRule)
{
    m_pinList.append(pinRule);
}
