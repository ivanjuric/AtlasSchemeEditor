#include <QString>
#include "Enums.h"
#include "rulecheck.h"
#include <QMap>

#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

class Attribute
{
public:
    Attribute();

    // Getters
    QString id() { return m_id; }
    QString title() { return m_title; }
    QString popupText() { return m_popupText; }
    PopupTypeEnum popupType() { return m_popupType; }
    int defaultValue() { return m_defaultValue; }
    int currentValue() { return m_currentValue; }
    QMap<QString, int> enumeratedValue() { return m_enumeratedValue; }
    bool isValueSet() { return m_isValueSet; }
    QVector<RuleCheck*> rules() { return m_rules; }

    // Setters
    void setId(QString id) { m_id = id; }
    void setTitle(QString title) { m_title = title; }
    void setPopupText(QString popupText) { m_popupText = popupText; }
    void setPopupType(PopupTypeEnum popupType) { m_popupType = popupType; }
    void setDefaultValue(int defaultValue) { m_defaultValue = defaultValue; }
    void setCurrentValue(int currentValue);
    void addEnumeratedValue(QString name, int value);
    void addRule(RuleCheck *rule);


private:
    QString m_id;
    QString m_title;
    QString m_popupText;
    PopupTypeEnum m_popupType;
    int m_defaultValue;
    int m_currentValue;
    QMap<QString, int> m_enumeratedValue;
    bool m_isValueSet;
    QVector<RuleCheck*> m_rules;
};

#endif // ATTRIBUTE_H
