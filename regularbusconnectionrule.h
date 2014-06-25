#include <QString>
#include <QVector>
#include "rulepopup.h"

#ifndef REGULARBUSCONNECTIONRULE_H
#define REGULARBUSCONNECTIONRULE_H

class RegularBusConnectionRule
{
public:
    RegularBusConnectionRule();

    // Getters
    QString componentId() { return m_componentId; }
    QString componentVisualPinId() { return m_componentVisualPinId; }
    QString busId() { return m_busId; }
    QVector<QString> pinInstantiationList() { return m_pinInstantiationList; }
    QVector<QString> connectionTerminates() { return m_connectionTerminates; }
    QVector<RulePopup*> popups() { return m_popups; }

    // Setters
    void setComponentId(QString componentId) { m_componentId = componentId; }
    void setComponentVisualPinId(QString componentVisualPinId) { m_componentVisualPinId = componentVisualPinId; }
    void setBusId(QString busId) { m_busId = busId; }
    void addPinIdToInstantiationList(QString pinId);
    void addToConnectionTerminates(QString pinId);
    void addPopup(RulePopup *popup);

private:
    QString m_componentId;
    QString m_componentVisualPinId;
    QString m_busId;
    QVector<QString> m_pinInstantiationList;
    QVector<QString> m_connectionTerminates;
    QVector<RulePopup*> m_popups;
};

#endif // REGULARBUSCONNECTIONRULE_H
