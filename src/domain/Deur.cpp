/**
 * @file Deur.cpp
 * @brief Implementatie van de Deur basis-klasse.
 *
 * STUB-fase: open() negeert nog het slot. Tests Deur.OpenWeigertBij...
 * zullen FAILEN totdat we de slot-check inbouwen (GREEN-fase).
 */

#include "Deur.h"

namespace domain {

Deur::Deur(int x, int y, unsigned lengte)
    : _status(false), _x_coordinaat(x), _y_coordinaat(y), _lengte(lengte)
    , _mijnSlot(nullptr)
{
}

void Deur::open()
{
    // Een deur met een vergrendeld slot kan niet open.
    // Geen slot (nullptr) = altijd ontgrendeld - dan mag de deur gewoon open.
    if (_mijnSlot && _mijnSlot->isVergrendeld()) {
        return;
    }
    _status = true;
}

void Deur::sluit()
{
    _status = false;
    // Per opdracht 2: wanneer een deur sluit, wordt het slot automatisch
    // vergrendeld. Volgende open() moet dus weer eerst ontgrendeld worden.
    if (_mijnSlot) {
        _mijnSlot->vergrendel();
    }
}

bool Deur::isDeurOpen() const
{
    return _status;
}

unsigned Deur::deurLengte() const
{
    return _lengte;
}

void Deur::setSlot(std::shared_ptr<Slot> slot)
{
    _mijnSlot = slot;
}

} // namespace domain
