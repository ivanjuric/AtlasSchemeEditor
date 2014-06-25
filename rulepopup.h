#include <QString>
#include "Enums.h"
#include "pinrulepopup.h"

#ifndef RULEPOPUP_H
#define RULEPOPUP_H

class RulePopup
{
public:
    RulePopup();

    // Getters
    QString id() { return m_id; }
    QString contextMenuItem() { return m_contextMenuItem; }
    PopupTypeEnum open() { return m_open; }
    QString message() { return m_message; }
    QString pinHeaderText() { return m_pinHeaderText; }
    QString busLineHeaderText() { return m_busLineHeaderText; }
    QVector<PinRulePopup*> pinList() { return m_pinList; }

    // Setters
    void setId(QString id) { m_id = id; }
    void setContextMenuItem(QString contextMenuItem) { m_contextMenuItem = contextMenuItem; }
    void setOpen(PopupTypeEnum open) { m_open = open; }
    void setMessage(QString message) { m_message = message; }
    void setPinHeaderText(QString pinHeaderText) { m_pinHeaderText = pinHeaderText; }
    void setBusLineHeaderText(QString busLineHeaderText) { m_busLineHeaderText = busLineHeaderText; }
    void addPinRulePopupToPinList(PinRulePopup *pinRule);

private:
    QString m_id;
    QString m_contextMenuItem;
    PopupTypeEnum m_open;
    QString m_message;
    QString m_pinHeaderText;
    QString m_busLineHeaderText;
    QVector<PinRulePopup*> m_pinList;

};

#endif // RULEPOPUP_H
