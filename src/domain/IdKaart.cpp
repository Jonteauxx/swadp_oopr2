/**
 * @file IdKaart.cpp
 * @brief Implementatie van IdKaart (GREEN-fase TDD).
 */

#include "IdKaart.h"

#include <algorithm>

namespace domain {

IdKaart::IdKaart(const std::string& id,
                 const std::string& naam,
                 const std::string& adres)
    : _id(id), _naamEigenaar(naam), _adresEigenaar(adres)
{
}

const std::string& IdKaart::userId() const         { return _id; }
const std::string& IdKaart::naamEigenaar() const   { return _naamEigenaar; }
const std::string& IdKaart::adresEigenaar() const  { return _adresEigenaar; }

void IdKaart::geefToegang(KaartSlot* slot)
{
    if (slot == nullptr) return;
    // Idempotent: dubbele toevoeging wordt vermeden.
    auto it = std::find(_toegang.begin(), _toegang.end(), slot);
    if (it == _toegang.end()) {
        _toegang.push_back(slot);
    }
}

void IdKaart::verwijderToegang(KaartSlot* slot)
{
    _toegang.erase(std::remove(_toegang.begin(), _toegang.end(), slot),
                   _toegang.end());
}

bool IdKaart::heeftToegangTot(KaartSlot* slot) const
{
    return std::find(_toegang.begin(), _toegang.end(), slot) != _toegang.end();
}

const std::vector<KaartSlot*>& IdKaart::toegang() const
{
    return _toegang;
}

std::set<KaartSlot*> operator+(const IdKaart& a, const IdKaart& b)
{
    std::set<KaartSlot*> result(a.toegang().begin(), a.toegang().end());
    result.insert(b.toegang().begin(), b.toegang().end());
    return result;
}

} // namespace domain
