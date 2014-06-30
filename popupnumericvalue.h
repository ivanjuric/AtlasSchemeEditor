#ifndef POPUPNUMERICVALUE_H
#define POPUPNUMERICVALUE_H

#include "componentview.h"

#include <QDialog>

namespace Ui {
class PopupNumericValue;
}

class PopupNumericValue : public QDialog
{
    Q_OBJECT

public:
    explicit PopupNumericValue(QWidget *parent = 0);
    ~PopupNumericValue();

    ComponentView *component() { return m_component; }
    Attribute *attribute() { return m_attribute; }
    QGraphicsScene *scene() { return m_scene; }

    void setComponent(ComponentView *component) { m_component = component; }
    void setComponentAttribute(Attribute *attribute) { m_attribute = attribute; }
    void setScene(QGraphicsScene *scene) { m_scene = scene;}
    void fillValues(bool init = false);
    bool checkRuleChecks(QString text);


private slots:

    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::PopupNumericValue *ui;

    ComponentView *m_component;
    Attribute *m_attribute;
    QGraphicsScene *m_scene;
};

#endif // POPUPNUMERICVALUE_H
