/**
 * @file Deur.cpp
 * @brief Implementatie van de Deur basis-klasse met 0..n sloten.
 */

#include "Deur.h"

namespace domain {

Deur::Deur(int x, int y, unsigned lengte)
    : _status(false)
    , _x_coordinaat(x), _y_coordinaat(y), _lengte(lengte)
{
}

void Deur::open()
{
    // ALLE sloten moeten ontgrendeld zijn. Eén vergrendeld slot is genoeg
    // om de deur dicht te houden.
    if (!alleSlotenOntgrendeld()) {
        return;
    }
    _status = true;
}

void Deur::sluit()
{
    _status = false;
    // Bij sluiten: ALLE sloten vergrendelen automatisch (opdracht 2 +3).
    for (auto& slot : _sloten) {
        if (slot) {
            slot->vergrendel();
        }
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

void Deur::voegSlotToe(std::shared_ptr<Slot> slot)
{
    if (slot) {
        _sloten.push_back(slot);
    }
}

bool Deur::alleSlotenOntgrendeld() const
{
    for (const auto& slot : _sloten) {
        if (slot && slot->isVergrendeld()) {
            return false;
        }
    }
    return true;
}

std::size_t Deur::aantalVergrendeld() const
{
    std::size_t n = 0;
    for (const auto& slot : _sloten) {
        if (slot && slot->isVergrendeld()) {
            ++n;
        }
    }
    return n;
}

std::size_t Deur::aantalSloten() const
{
    return _sloten.size();
}

} // namespace domain
