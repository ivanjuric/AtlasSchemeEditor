#include "popupenumeratedvalue.h"
#include "ui_popupenumeratedvalue.h"

#include <QRadioButton>

PopupEnumeratedValue::PopupEnumeratedValue(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PopupEnumeratedValue)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

void PopupEnumeratedValue::fillValues(bool init)
{
    if(m_attribute)
    {
        setWindowTitle(m_attribute->title());
        ui->label->setText(m_attribute->popupText());
        populateRadioButtons(m_attribute->enumeratedValue(), init);
    }
}

void PopupEnumeratedValue::populateRadioButtons(QMap<QString, int> enumeratedValue, bool init)
{
    group = new QButtonGroup();
    foreach (int val, enumeratedValue.values())
    {
        QRadioButton *radio = new QRadioButton(enumeratedValue.key(val));
        group->addButton(radio);
        group->setId(radio, val);
        if(init)
        {
            if(m_attribute->defaultValue() == val)
                radio->setChecked(true);
        }
        else
        {
            if(m_attribute->currentValue() == val)
                radio->setChecked(true);
        }
        ui->verticalGroupBox->layout()->addWidget(radio);
    }
}

PopupEnumeratedValue::~PopupEnumeratedValue()
{
    delete ui;
}

void PopupEnumeratedValue::on_okButton_clicked()
{
    m_attribute->setCurrentValue(group->checkedId());
    accept();
}

void PopupEnumeratedValue::on_cancelButton_clicked()
{
    reject();
}
