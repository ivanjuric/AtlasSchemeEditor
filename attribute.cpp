#include "attribute.h"

Attribute::Attribute()
{
    m_currentValue = 0;
    m_isValueSet = false;
}

void Attribute::addEnumeratedValue(QString name, int value)
{
    m_enumeratedValue[name] = value;
}
void Attribute::setCurrentValue(int currentValue)
{
    m_currentValue = currentValue;
    m_isValueSet = true;
}
void Attribute::addRule(RuleCheck* rule)
{
    m_rules.append(rule);
}
