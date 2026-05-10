/**
 * @file Draaideur.cpp
 * @brief Implementatie van Draaideur.
 */

#include "Draaideur.h"

#include <QPaintDevice>
#include <QPainter>
#include <QPen>

namespace domain {

Draaideur::Draaideur(int x, int y, unsigned lengte)
    : Deur(x, y, lengte), _liggend(false)
{
}

void Draaideur::open()
{
    Deur::open();
    _liggend = true;
}

void Draaideur::sluit()
{
    Deur::sluit();
    _liggend = false;
}

bool Draaideur::isLiggend() const
{
    return _liggend;
}

void Draaideur::teken(QPaintDevice* target)
{
    QPainter painter(target);

    QPen pen(Qt::black, 4, Qt::SolidLine, Qt::FlatCap);
    painter.setPen(pen);

    const int L = static_cast<int>(_lengte);

    if (_liggend) {
        // Open: horizontaal naar rechts (haaks op de verticale muur).
        painter.drawLine(_x_coordinaat,     _y_coordinaat,
                         _x_coordinaat + L, _y_coordinaat);
    } else {
        // Dicht: verticaal omlaag (in lijn met de muur).
        painter.drawLine(_x_coordinaat, _y_coordinaat,
                         _x_coordinaat, _y_coordinaat + L);
    }
}

} // namespace domain
