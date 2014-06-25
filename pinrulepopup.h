#include <QString>
#include <QVector>

#ifndef PINRULEPOPUP_H
#define PINRULEPOPUP_H

class PinRulePopup
{
public:
    PinRulePopup();
    // Getters
    QString id() { return m_id; }
    QString title() { return m_title; }
    QString defaultPin() { return m_defaultPin; }
    QVector<QString> busLineList() { return m_busLineList; }

    // Setters
    void setId(QString id) { m_id = id; }
    void setTitle(QString title) { m_title = title; }
    void setDefaultPin(QString defaultPin) { m_defaultPin = defaultPin; }
    void addBusLineToList(QString busLineId);

private:
    QString m_id;
    QString m_title;
    QString m_defaultPin;
    QVector<QString> m_busLineList;

};

#endif // PINRULEPOPUP_H
