#include "bus.h"

Bus::Bus()
{
}
void Bus::addBusLine(BusLine *busLine)
{
    m_busLines.append(busLine);
}
