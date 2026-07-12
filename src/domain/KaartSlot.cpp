/**
 * @file KaartSlot.cpp
 * @brief Implementatie van KaartSlot (GREEN-fase TDD).
 */

#include "KaartSlot.h"

#include "IdKaart.h"

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
        return; // onbekende kaart-id
    }
    IdKaart* kaart = it->second;
    if (kaart && kaart->heeftToegangTot(this)) {
        _vergrendeld = false;
    }
    // Anders: kaart bestaat maar heeft geen toegang tot dit slot.
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
