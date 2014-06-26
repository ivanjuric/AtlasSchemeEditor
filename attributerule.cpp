#include "attributerule.h"
#include "libraryfile.h"

AttributeRule::AttributeRule()
{
    m_type = (AttributeRuleTypeEnum)0;
    m_condition = (AttributeRuleConditionEnum)0;
}
void AttributeRule::addParameter(QString parameter)
{
    m_parameters.append(parameter);
}
QString AttributeRule::createErrorMessage()
{


    return "";
}
bool AttributeRule::check(LibraryFile *library, QGraphicsScene *scene)
{


    return true;
}
