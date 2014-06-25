#include "attribute.h"

Attribute::Attribute()
{
}

void Attribute::addEnumeratedValue(QString name, int value)
{
    m_enumeratedValue[name] = value;
}
