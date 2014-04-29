#include "util.h"

Util::Util()
{
}
// Read config file and create json object
bool Util::ParseJson(QJsonObject *json, QString filepath)
{
    QFile configFile(filepath);
    if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QString data = configFile.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(data.toUtf8()));
    QJsonObject library = doc.object();
    QString glos = library["glossary"].toObject()["title"].toString();

    return true;
}
