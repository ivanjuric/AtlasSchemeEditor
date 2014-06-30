#include <QString>
#include <QVector>

#ifndef AUTOMATICCONNECTION_H
#define AUTOMATICCONNECTION_H

class AutomaticConnection
{
public:
    AutomaticConnection();

    QString popupOptionConnect() { return m_popupOptionConnect; }
    QString popupOptionContinue() { return m_popupOptionContinue; }
    QString contextMenuItemText() { return m_contextMenuItemText; }
    QString automaticBus() { return m_automaticBus; }
    QVector<QString> pinInstantiationList() { return m_pinInstantiationList; }

    void setPopupOptionConnect(QString popupOptionConnect) { m_popupOptionConnect = popupOptionConnect; }
    void setPopupOptionContinue(QString popupOptionContinue) { m_popupOptionContinue = popupOptionContinue; }
    void setContextMenuItemText(QString contextMenuItemText) { m_contextMenuItemText = contextMenuItemText; }
    void setAutomaticBus(QString automaticBus) { m_automaticBus = automaticBus; }
    void addToPinInstantiationList(QString pin);

private:
    QString m_popupOptionConnect;
    QString m_popupOptionContinue;
    QString m_contextMenuItemText;
    QString m_automaticBus;
    QVector<QString> m_pinInstantiationList;
};

#endif // AUTOMATICCONNECTION_H
