#include "componentmodel.h"

//Default constructor
ComponentModel::ComponentModel()
{
}

void ComponentModel::addPin(PinModel *pin)
{
    m_pins.append(pin);
}

void ComponentModel::addVisualElement(VisualComponentElement *element)
{
    VisualRectangle *r = dynamic_cast<VisualRectangle*>(element);
    VisualText *t = dynamic_cast<VisualText*>(element);
    VisualCircle *c = dynamic_cast<VisualCircle*>(element);
    if(r)
        addRectangle(r);
    else if(c)
        addCircle(c);
    else if(t)
        addText(t);
}
void ComponentModel::addAttribute(Attribute* attribute)
{
    m_attributes.append(attribute);
}

void ComponentModel::addRectangle(VisualRectangle *rect)
{
    VisualRectangle *r = new VisualRectangle(rect);
    m_visualElements.append(r);
}
void ComponentModel::addCircle(VisualCircle *circle)
{
    VisualCircle *c = new VisualCircle(circle);
    m_visualElements.append(c);
}
void ComponentModel::addText(VisualText *text)
{
    VisualText *t = new VisualText(text);
    m_visualElements.append(t);
}
