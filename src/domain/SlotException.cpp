/**
 * @file SlotException.cpp
 * @brief Implementatie van SlotException (GREEN-fase TDD, opdracht 6).
 */

#include "SlotException.h"

namespace domain {

SlotException::SlotException(const std::string& slotPlaats,
                             const std::string& kaarId)
    : _slotPlaats(slotPlaats)
    , _kaarId(kaarId)
    , _bericht("SlotException @ plaats '" + slotPlaats + "' - id: " + kaarId)
{
}

const std::string& SlotException::plaatsVanHetSlot() const noexcept
{
    return _slotPlaats;
}

const std::string& SlotException::kaartVanBinnendringer() const noexcept
{
    return _kaarId;
}

const char* SlotException::what() const noexcept
{
    return _bericht.c_str();
}

} // namespace domain
