/**
 * @file KaartSlot.cpp
 * @brief Implementatie van KaartSlot (GREEN-fase TDD).
 */

#include "KaartSlot.h"

#include "IdKaart.h"
#include "SlotException.h"

namespace domain {

std::map<std::string, IdKaart*> KaartSlot::_idKaarten;

KaartSlot::KaartSlot(const std::string& plaats)
    : _plaats(plaats), _vergrendeld(true)
{
}

void KaartSlot::ontgrendel(const std::string& kaartId)
{
    auto it = _idKaarten.find(kaartId);
    if (it == _idKaarten.end()) {
        // Onbekende id: exception met de plaats van dit slot en de tekst
        // "geen idkaart voor xxxx" (opdracht 6).
        throw SlotException(_plaats, "geen idkaart voor " + kaartId);
    }
    IdKaart* kaart = it->second;
    if (!kaart || !kaart->heeftToegangTot(this)) {
        // Kaart bestaat maar heeft geen toegang tot dit slot: exception met
        // de id van de IdKaart en de plaats van het KaartSlot (opdracht 6).
        const std::string id = kaart ? kaart->userId()
                                     : "geen idkaart voor " + kaartId;
        throw SlotException(_plaats, id);
    }
    _vergrendeld = false;
}

void KaartSlot::vergrendel()
{
    _vergrendeld = true;
}

bool KaartSlot::isVergrendeld() const
{
    return _vergrendeld;
}

const std::string& KaartSlot::plaats() const
{
    return _plaats;
}

void KaartSlot::voegIdKaartToe(IdKaart* kaart)
{
    if (kaart == nullptr) return;
    _idKaarten[kaart->userId()] = kaart;
}

void KaartSlot::verwijderIdKaart(const std::string& eenId)
{
    _idKaarten.erase(eenId);
}

std::vector<IdKaart*> KaartSlot::zoekIdKaarten(
    const std::function<bool(const IdKaart&)>& predicate)
{
    std::vector<IdKaart*> result;
    for (const auto& [id, kaart] : _idKaarten) {
        if (kaart && predicate(*kaart)) {
            result.push_back(kaart);
        }
    }
    return result;
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
