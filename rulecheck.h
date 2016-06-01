#include "Enums.h"
#include <QString>
#include <QVector>

#ifndef RULECHECK_H
#define RULECHECK_H

class LibraryFile;
class QGraphicsScene;

class RuleCheck
{
public:
    RuleCheck();

    RuleCheckTypeEnum type() { return m_type; }
    RuleCheckConditionEnum condition() { return m_condition; }
    QVector<QString> parameters() { return m_parameters; }
    QString errorMessage() { return m_errorMessage; }

    void setType(RuleCheckTypeEnum type) { m_type = type; }
    void setCondition(RuleCheckConditionEnum condition) { m_condition = condition; }
    void setErrorMessage(QString errorMessage) { m_errorMessage = errorMessage; }
    void addParameter(QString parameter);

    QString createErrorMessage();

    // Attribute rule checks
    bool checkDivisibleByPowerOf2(QVector<QString> paramters);
    bool checkGreaterOrEqual(QVector<QString> paramters);
    bool checkLessOrEqual(QVector<QString> paramters);
    bool checkStartsInAddressSpace(QVector<QString> paramters);
    bool checkEndsInAddressSpace(QVector<QString> paramters);
    bool checkDivisibleBy(QVector<QString> paramters);
    bool checkInRange(QVector<QString> paramters);


private:
    RuleCheckTypeEnum m_type;
    RuleCheckConditionEnum m_condition;
    QVector<QString> m_parameters;
    QString m_errorMessage;
};

#endif // RULECHECK_H
