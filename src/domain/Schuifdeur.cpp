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
    // Bij open is de schuifdeur volledig in de muur geschoven en dus
    // niet zichtbaar. Het gat in de plattegrond-PNG blijft dan zichtbaar
    // als duidelijke opening.
    if (_status) {
        return;
    }

    QPainter painter(target);

    QPen pen(Qt::black, 4, Qt::SolidLine, Qt::FlatCap);
    painter.setPen(pen);

    painter.drawLine(_x_coordinaat, _y_coordinaat,
                     _x_coordinaat,
                     _y_coordinaat + static_cast<int>(_lengte));
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
