#ifndef POPUPENUMERATEDVALUE_H
#define POPUPENUMERATEDVALUE_H

#include "componentview.h"

#include <QButtonGroup>
#include <QDialog>

namespace Ui {
class PopupEnumeratedValue;
}

class PopupEnumeratedValue : public QDialog
{
    Q_OBJECT

public:
    explicit PopupEnumeratedValue(QWidget *parent = 0);
    ~PopupEnumeratedValue();

    ComponentView *component() { return m_component; }
    Attribute *attribute() { return m_attribute; }
    QGraphicsScene *scene() { return m_scene; }

    void setComponent(ComponentView *component) { m_component = component; }
    void setComponentAttribute(Attribute *attribute) { m_attribute = attribute; }
    void setScene(QGraphicsScene *scene) { m_scene = scene;}
    void fillValues(bool init = false);
    bool checkAttributeRules(QString text);

private slots:
    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::PopupEnumeratedValue *ui;

    ComponentView *m_component;
    Attribute *m_attribute;
    QGraphicsScene *m_scene;

    void populateRadioButtons(QMap<QString, int> enumeratedValue, bool init = false);
    QButtonGroup *group;
};

#endif // POPUPENUMERATEDVALUE_H
