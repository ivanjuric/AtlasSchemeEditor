#include "popupnumericvalue.h"
#include "ui_popupnumericvalue.h"

#include <QMessageBox>

PopupNumericValue::PopupNumericValue(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PopupNumericValue)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

PopupNumericValue::~PopupNumericValue()
{
    delete ui;
}
void PopupNumericValue::fillValues(bool init)
{
    if(m_attribute)
    {
        setWindowTitle(m_attribute->title());
        ui->label->setText(m_attribute->popupText());
        if(init)
            ui->lineEdit->setText(QString::number(m_attribute->defaultValue()));
        else
            ui->lineEdit->setText(QString::number(m_attribute->currentValue()));
    }
}
bool PopupNumericValue::checkAttributeRules(QString text)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Invalid input parameter");

    // Parse entered value to int value if base is 10 or 16 (hex)
    bool okInt = false, okHex = false;
    int intValue = 0, hexValue = 0;
    bool isHexType = text.left(2) == "0x" || text.left(2) == "0X";
    if(isHexType)
        hexValue = text.toInt(&okHex,16);
    else
        intValue = text.toInt(&okInt,10);

    // Check if entered value is valid
    if((!isHexType && !okInt) || (isHexType && !okHex))
    {
        msgBox.setText("Entered value is not \"int\" or \"hex\" value!");
        msgBox.exec();
        return false;
    }

    Attribute *a = m_attribute;
    foreach (AttributeRule *rule, a->rules())
    {

    }

    return true;
}


void PopupNumericValue::on_okButton_clicked()
{
    QString text = ui->lineEdit->text();
    if(checkAttributeRules(text))
    {
        bool ok;
        if(text.left(2) == "0x" || text.left(2) == "0X")
            m_attribute->setCurrentValue(text.toInt(&ok,16));
        else
            m_attribute->setCurrentValue(text.toInt(&ok,10));

        accept();
    }
}

void PopupNumericValue::on_cancelButton_clicked()
{
    Attribute *a = m_attribute;
    if(a->isValueSet())
        m_attribute->setCurrentValue(a->currentValue());
    else
        m_attribute->setCurrentValue(a->defaultValue());
    reject();
}
