/**
 * @file KaartSlot.cpp
 * @brief STUB-implementatie van KaartSlot (RED-fase TDD).
 */

#include "KaartSlot.h"

#include "IdKaart.h"

namespace domain {

// Definitie van de statische map (één instance voor het hele programma).
std::map<std::string, IdKaart*> KaartSlot::_idKaarten;

KaartSlot::KaartSlot(const std::string& plaats)
    : _plaats(plaats), _vergrendeld(true)
{
}

void KaartSlot::ontgrendel(const std::string& /*kaartId*/)
{
    // STUB: hoort op te zoeken in _idKaarten en te checken heeftToegangTot(this).
}

void KaartSlot::vergrendel()
{
    // STUB: hoort _vergrendeld = true te zetten.
}

bool KaartSlot::isVergrendeld() const
{
    return _vergrendeld;
}

const std::string& KaartSlot::plaats() const
{
    return _plaats;
}

void KaartSlot::voegIdKaartToe(IdKaart* /*kaart*/)
{
    // STUB: hoort _idKaarten[kaart->userId()] = kaart te doen.
}

void KaartSlot::verwijderIdKaart(const std::string& /*eenId*/)
{
    // STUB: hoort _idKaarten.erase(eenId) te doen.
}

std::vector<IdKaart*> KaartSlot::zoekIdKaarten(
    const std::function<bool(const IdKaart&)>& /*predicate*/)
{
    // STUB: hoort de map af te lopen met predicate.
    return {};
}

void KaartSlot::wisAlleIdKaarten()
{
    _idKaarten.clear();
}

std::size_t KaartSlot::aantalIdKaarten()
{
    return _idKaarten.size();
}

} // namespace domain
