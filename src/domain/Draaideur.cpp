/**
 * @file Draaideur.cpp
 * @brief Implementatie van Draaideur.
 */

#include "Draaideur.h"

namespace domain {

Draaideur::Draaideur(int x, int y, unsigned lengte,
                     Orientatie orientatie, ZwaaiRichting richting)
    : Deur(x, y, lengte)
    , _liggend(false)
    , _orientatie(orientatie)
    , _zwaaiRichting(richting)
{
}

void Draaideur::open()
{
    Deur::open();
    // _liggend moet de werkelijke status volgen: als Deur::open() geweigerd
    // is (vergrendeld slot), dan blijft _liggend false en tekent de deur
    // nog steeds als dicht. Bug-fix t.o.v. eerder altijd-true zetten.
    _liggend = _status;
}

void Draaideur::sluit()
{
    Deur::sluit();
    _liggend = _status;
}

bool Draaideur::isLiggend() const
{
    return _liggend;
}

} // namespace domain
