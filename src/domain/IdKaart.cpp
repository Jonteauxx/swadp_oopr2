/**
 * @file IdKaart.cpp
 * @brief STUB-implementatie van IdKaart (RED-fase TDD).
 */

#include "IdKaart.h"

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

void IdKaart::geefToegang(KaartSlot* /*slot*/)
{
    // STUB: hoort _toegang.push_back(slot) te doen (uniek).
}

void IdKaart::verwijderToegang(KaartSlot* /*slot*/)
{
    // STUB: hoort uit _toegang weg te halen.
}

bool IdKaart::heeftToegangTot(KaartSlot* /*slot*/) const
{
    // STUB: hoort te checken of slot in _toegang voorkomt.
    return false;
}

const std::vector<KaartSlot*>& IdKaart::toegang() const
{
    return _toegang;
}

std::set<KaartSlot*> operator+(const IdKaart& /*a*/, const IdKaart& /*b*/)
{
    // STUB: hoort vereniging van a.toegang() en b.toegang() te geven.
    return {};
}

} // namespace domain
