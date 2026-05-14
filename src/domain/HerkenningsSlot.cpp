/**
 * @file HerkenningsSlot.cpp
 * @brief STUB-implementatie van HerkenningsSlot (RED-fase TDD).
 *        Tests zullen FAILEN totdat we de echte logica inbouwen.
 */

#include "HerkenningsSlot.h"

namespace domain {

HerkenningsSlot::HerkenningsSlot(Afdrukker* afdrukker)
    : _vergrendeld(true)
    , _mijnAfdrukker(afdrukker)
{
}

void HerkenningsSlot::ontgrendel(const std::string& /*naam*/)
{
    // STUB: hoort in kaartenbak op te zoeken of naam geautoriseerd is.
}

void HerkenningsSlot::vergrendel()
{
    // STUB: hoort _vergrendeld op true te zetten.
}

bool HerkenningsSlot::isVergrendeld() const
{
    return _vergrendeld;
}

void HerkenningsSlot::voegAutorissatieToe(const std::string& /*naam*/,
                                          bool /*magNaarBinnen*/)
{
    // STUB: hoort _kaartenbak[naam] = magNaarBinnen te doen.
}

void HerkenningsSlot::toonKaartenbak()
{
    // STUB: hoort via _mijnAfdrukker de inhoud te tonen.
}

} // namespace domain
