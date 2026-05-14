/**
 * @file HerkenningsSlot.cpp
 * @brief Implementatie van HerkenningsSlot (GREEN-fase TDD).
 */

#include "HerkenningsSlot.h"

namespace domain {

HerkenningsSlot::HerkenningsSlot(Afdrukker* afdrukker)
    : _vergrendeld(true)
    , _mijnAfdrukker(afdrukker)
{
}

void HerkenningsSlot::ontgrendel(const std::string& naam)
{
    auto it = _kaartenbak.find(naam);
    if (it != _kaartenbak.end() && it->second == true) {
        _vergrendeld = false;
    }
    // Onbekend of geblokkeerd: blijft vergrendeld.
}

void HerkenningsSlot::vergrendel()
{
    _vergrendeld = true;
}

bool HerkenningsSlot::isVergrendeld() const
{
    return _vergrendeld;
}

void HerkenningsSlot::voegAutorissatieToe(const std::string& naam, bool magNaarBinnen)
{
    _kaartenbak[naam] = magNaarBinnen;
}

void HerkenningsSlot::toonKaartenbak()
{
    if (_mijnAfdrukker == nullptr) {
        return; // geen medium om op te tonen
    }

    _mijnAfdrukker->clearMedium();
    _mijnAfdrukker->toonText("=== Kaartenbak ===");
    for (const auto& [naam, magBinnen] : _kaartenbak) {
        const std::string regel = naam + " : "
                                + (magBinnen ? "TOEGANG" : "GEBLOKKEERD");
        _mijnAfdrukker->toonText(regel);
    }
}

} // namespace domain
