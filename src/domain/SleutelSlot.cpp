/**
 * @file SleutelSlot.cpp
 * @brief STUB-implementatie van SleutelSlot.
 *        Tests zullen FAILEN met deze versie (RED-fase TDD).
 */

#include "SleutelSlot.h"

namespace domain {

SleutelSlot::SleutelSlot(const std::string& sleutel)
    : _sleutel(sleutel)
    , _vergrendeld(true)   // start vergrendeld - dat IS correct gedrag
{
}

void SleutelSlot::ontgrendel(const std::string& /*eenSleutel*/)
{
    // STUB: hoort _vergrendeld = false te zetten als eenSleutel == _sleutel.
}

void SleutelSlot::vergrendel()
{
    // STUB: hoort _vergrendeld = true te zetten.
}

bool SleutelSlot::isVergrendeld() const
{
    return _vergrendeld;
}

} // namespace domain
