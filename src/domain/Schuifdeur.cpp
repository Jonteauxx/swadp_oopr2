/**
 * @file Schuifdeur.cpp
 * @brief Implementatie van Schuifdeur (teken + conditionele sluit).
 */

#include "Schuifdeur.h"
#include "Sensor.h"

namespace domain {

Schuifdeur::Schuifdeur(int x, int y, unsigned lengte, Sensor* sensor)
    : Deur(x, y, lengte), _mijnSensor(sensor)
{
}

void Schuifdeur::sluit()
{
    if (_mijnSensor != nullptr && _mijnSensor->isGeactiveerd()) {
        // Hal-sensor actief = magneet drukt aan = deur "denkt" dat hij dicht is,
        // dus we blijven in de open-staat.
        return;
    }
    Deur::sluit();
}

} // namespace domain
