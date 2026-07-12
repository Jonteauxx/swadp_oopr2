/**
 * @file SlotException.cpp
 * @brief STUB (TDD RED-fase) - bewaart nog niets, geeft lege waarden terug.
 *
 * Deze stub compileert zodat de tests kunnen linken, maar geeft expres
 * verkeerde (lege) resultaten. De bijbehorende tests horen daardoor te
 * FALEN. In de GREEN-fase wordt dit correct geimplementeerd.
 */

#include "SlotException.h"

namespace domain {

SlotException::SlotException(const std::string& /*plaats*/,
                             const std::string& /*id*/)
{
    // STUB: bewaart nog niets.
}

const std::string& SlotException::plaats() const noexcept
{
    return _plaats; // STUB: altijd leeg
}

const std::string& SlotException::id() const noexcept
{
    return _id; // STUB: altijd leeg
}

const char* SlotException::what() const noexcept
{
    return _bericht.c_str(); // STUB: altijd leeg
}

} // namespace domain
