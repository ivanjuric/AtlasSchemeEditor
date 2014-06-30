#include "rulecheck.h"
#include "libraryfile.h"

RuleCheck::RuleCheck()
{
    m_type = (RuleCheckTypeEnum)0;
    m_condition = (RuleCheckConditionEnum)0;
}
void RuleCheck::addParameter(QString parameter)
{
    m_parameters.append(parameter);
}
QString RuleCheck::createErrorMessage()
{


    return "";
}
bool RuleCheck::checkDivisibleByPowerOf2(QVector<QString> paramters)
{


    return true;
}
bool RuleCheck::checkGreaterOrEqual(QVector<QString> paramters)
{


    return true;
}
bool RuleCheck::checkLessOrEqual(QVector<QString> paramters)
{


    return true;
}
bool RuleCheck::checkStartsInAddressSpace(QVector<QString> paramters)
{


    return true;
}
bool RuleCheck::checkEndsInAddressSpace(QVector<QString> paramters)
{


    return true;
}
bool RuleCheck::checkDivisibleBy(QVector<QString> paramters)
{


    return true;
}
bool RuleCheck::checkInRange(QVector<QString> paramters)
{


    return true;
}
