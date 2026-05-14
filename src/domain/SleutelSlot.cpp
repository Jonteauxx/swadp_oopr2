/**
 * @file SleutelSlot.cpp
 * @brief Implementatie van SleutelSlot (GREEN fase TDD).
 */

#include "SleutelSlot.h"

namespace domain {

SleutelSlot::SleutelSlot(const std::string& sleutel)
    : _sleutel(sleutel)
    , _vergrendeld(true)
{
}

void SleutelSlot::ontgrendel(const std::string& eenSleutel)
{
    if (eenSleutel == _sleutel) {
        _vergrendeld = false;
    }
    // Bij verkeerde sleutel verandert er niets (blijft vergrendeld).
}

void SleutelSlot::vergrendel()
{
    _vergrendeld = true;
}

bool SleutelSlot::isVergrendeld() const
{
    return _vergrendeld;
}

} // namespace domain
