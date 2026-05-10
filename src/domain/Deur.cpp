/**
 * @file Deur.cpp
 * @brief Implementatie van de Deur basis-klasse.
 */

#include "Deur.h"

namespace domain {

Deur::Deur(int x, int y, unsigned lengte)
    : _status(false), _x_coordinaat(x), _y_coordinaat(y), _lengte(lengte)
{
}

void Deur::open()
{
    _status = true;
}

void Deur::sluit()
{
    _status = false;
}

bool Deur::isDeurOpen() const
{
    return _status;
}

unsigned Deur::deurLengte() const
{
    return _lengte;
}

} // namespace domain
