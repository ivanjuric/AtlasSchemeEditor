#include "Enums.h"
#include "automaticconnection.h"
#include <QString>

#ifndef CHECKCONNECTION_H
#define CHECKCONNECTION_H

class CheckConnection
{
public:
    CheckConnection();

    CheckConnectionTypeEnum type() { return m_type; }
    QString message() { return m_message; }
    AutomaticConnection* automaticConnection() { return m_automaticConnection; }

    void setType(CheckConnectionTypeEnum type) { m_type = type; }
    void setMessage(QString message) { m_message = message; }
    void setAutomaticConnection(AutomaticConnection *automaticConnection) { m_automaticConnection = automaticConnection; }

private:
    CheckConnectionTypeEnum m_type;
    QString m_message;
    AutomaticConnection *m_automaticConnection;
};

#endif // CHECKCONNECTION_H
