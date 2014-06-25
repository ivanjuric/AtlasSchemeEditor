#include <QString>
#include <QVector>

#ifndef AUTOMATICBUSCONNECTIONRULECOMPONENT_H
#define AUTOMATICBUSCONNECTIONRULECOMPONENT_H

class AutomaticBusConnectionRuleComponent
{
public:
    AutomaticBusConnectionRuleComponent();
    // Getters
    QString componentId() { return m_componentId; }
    QString componentVisualPinId() { return m_componentVisualPinId; }
    QVector<QString> pinInstantiationList() { return m_pinInstantiationList; }

    // Setters
    void setComponentId(QString componentId) { m_componentId = componentId; }
    void setComponentVisualPinId(QString componentVisualPinId) { m_componentVisualPinId = componentVisualPinId; }
    void addPinIdToInstantiationList(QString pinId);
private:
    QString m_componentId;
    QString m_componentVisualPinId;
    QVector<QString> m_pinInstantiationList;
};

#endif // AUTOMATICBUSCONNECTIONRULECOMPONENT_H
