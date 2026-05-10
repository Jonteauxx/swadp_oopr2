/**
 * @file Sensor.cpp
 * @brief Implementatie van de Sensor basisklasse.
 */

#include "Sensor.h"

namespace domain {

Sensor::Sensor(int x, int y)
    : _x(x), _y(y), _geactiveerd(false)
{
}

void Sensor::activeer()
{
    _geactiveerd = true;
}

void Sensor::deactiveer()
{
    _geactiveerd = false;
}

bool Sensor::isGeactiveerd() const
{
    return _geactiveerd;
}

std::pair<int, int> Sensor::coordinaten() const
{
    return { _x, _y };
}

} // namespace domain
