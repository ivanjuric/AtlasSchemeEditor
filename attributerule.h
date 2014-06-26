#include "Enums.h"
#include <QString>
#include <QVector>

#ifndef ATTRIBUTERULE_H
#define ATTRIBUTERULE_H

class LibraryFile;
class QGraphicsScene;

class AttributeRule
{
public:
    AttributeRule();

    AttributeRuleTypeEnum type() { return m_type; }
    AttributeRuleConditionEnum condition() { return m_condition; }
    QVector<QString> parameters() { return m_parameters; }
    QString errorMessage() { return m_errorMessage; }

    void setType(AttributeRuleTypeEnum type) { m_type = type; }
    void setCondition(AttributeRuleConditionEnum condition) { m_condition = condition; }
    void setErrorMessage(QString errorMessage) { m_errorMessage = errorMessage; }
    void addParameter(QString parameter);

    QString createErrorMessage();
    bool check(LibraryFile *library, QGraphicsScene *scene);


private:
    AttributeRuleTypeEnum m_type;
    AttributeRuleConditionEnum m_condition;
    QVector<QString> m_parameters;
    QString m_errorMessage;
};

#endif // ATTRIBUTERULE_H
