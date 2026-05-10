/**
 * @file Schuifdeur.cpp
 * @brief Implementatie van Schuifdeur (teken + conditionele sluit).
 */

#include "Schuifdeur.h"
#include "Sensor.h"

#include <QPaintDevice>
#include <QPainter>
#include <QPen>

namespace domain {

Schuifdeur::Schuifdeur(int x, int y, unsigned lengte, Sensor* sensor)
    : Deur(x, y, lengte), _mijnSensor(sensor)
{
}

void Schuifdeur::teken(QPaintDevice* target)
{
    QPainter painter(target);

    QPen pen(Qt::black, 4, Qt::SolidLine, Qt::FlatCap);
    painter.setPen(pen);

    // Dicht = volle lengte (vult het muurgat).
    // Open  = halve lengte (de andere helft is "weggeschoven").
    const int eindY = _y_coordinaat
                    + (_status ? static_cast<int>(_lengte) / 2
                               : static_cast<int>(_lengte));

    painter.drawLine(_x_coordinaat, _y_coordinaat,
                     _x_coordinaat, eindY);
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
