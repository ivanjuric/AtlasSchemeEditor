#include "automaticbusconnectionrulecomponent.h"

#include <QString>
#include <QVector>

#ifndef AUTOMATICBUSCONNECTIONRULE_H
#define AUTOMATICBUSCONNECTIONRULE_H

class AutomaticBusConnectionRule
{
public:
    AutomaticBusConnectionRule();

    // Getters
    QString busId() { return m_busId; }
    AutomaticBusConnectionRuleComponent* firstComponent() { return m_firstComponent; }
    AutomaticBusConnectionRuleComponent* secondComponent() { return m_secondComponent; }

    // Setters
    void setBusId(QString busId) { m_busId = busId; }
    void setFirstComponent(AutomaticBusConnectionRuleComponent *firstComponent) { m_firstComponent = firstComponent; }
    void setSecondComponent(AutomaticBusConnectionRuleComponent *secondComponent) { m_secondComponent = secondComponent; }

private:
    QString m_busId;
    AutomaticBusConnectionRuleComponent *m_firstComponent;
    AutomaticBusConnectionRuleComponent *m_secondComponent;
};

#endif // AUTOMATICBUSCONNECTIONRULE_H
