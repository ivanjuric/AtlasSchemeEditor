#include "pinview.h"
#include "visualcircle.h"
#include "visualcomponentelement.h"
#include "visualrectangle.h"
#include "visualtext.h"

#include <QString>
#include <QVector>

#ifndef COMPONENTMODEL_H
#define COMPONENTMODEL_H

class ComponentModel
{
public:
    // Constructor
    ComponentModel();

    // Getters
    QString id() { return m_id; }
    QString title() { return m_title; }
    QString instanceNameBase() { return m_instanceNameBase; }
    QString iconFile() { return m_iconFile; }
    QString tooltip() { return m_tooltip; }
    int minInstances() { return m_minInstances; }
    int maxInstances() { return m_maxInstances; }
    QString comdelFile() { return m_comdelFile; }
    QVector<VisualComponentElement*> visualElements() { return m_visualElements; }
    QVector<PinModel*> pins() { return m_pins; }

    // Setters
    void setId(QString id) { m_id = id; }
    void setTitle(QString title) { m_title = title; }
    void setInstanceNameBase(QString instanceNameBase) { m_instanceNameBase = instanceNameBase; }
    void setIconFile(QString iconFile) { m_iconFile = iconFile; }
    void setTooltip(QString tooltip) { m_tooltip = tooltip; }
    void setMinInstances(int minInstances) { m_minInstances = minInstances; }
    void setMaxInstances(int maxInstances) { m_maxInstances = maxInstances; }
    void setComdelFile(QString comdelFile) { m_comdelFile = comdelFile; }


    // Methods
    void addPin(PinModel *pin);
    void addVisualElement(VisualComponentElement *element);

private:
    QString m_id;
    QString m_title;
    QString m_instanceNameBase;
    QString m_iconFile;
    QString m_tooltip;
    int m_minInstances;
    int m_maxInstances;
    QString m_comdelFile;
    QVector<VisualComponentElement*> m_visualElements;
    QVector<PinModel*> m_pins;

private:
    // Helper methods for copying from existing model


    void addRectangle(VisualRectangle *rect);
    void addCircle(VisualCircle *circle);
    void addText(VisualText *text);
};

#endif // ComponentModel_H
