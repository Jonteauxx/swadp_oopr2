/**
 * @file CodeSlot.cpp
 * @brief Implementatie van CodeSlot (GREEN fase TDD).
 */

#include "CodeSlot.h"

#include <stdexcept>

namespace domain {

CodeSlot::CodeSlot(int code)
    : _code(code)
    , _vergrendeld(true)
{
}

void CodeSlot::ontgrendel(const std::string& eenCode)
{
    try {
        // std::stoi gooit std::invalid_argument bij niet-numerieke input,
        // en std::out_of_range bij te grote getallen.
        int ingevoerd = std::stoi(eenCode);
        if (ingevoerd == _code) {
            _vergrendeld = false;
        }
    } catch (const std::invalid_argument&) {
        // Niet-numerieke string -> negeren, blijft vergrendeld.
    } catch (const std::out_of_range&) {
        // Numerieke string maar buiten int-bereik -> negeren.
    }
}

void CodeSlot::vergrendel()
{
    _vergrendeld = true;
}

bool CodeSlot::isVergrendeld() const
{
    return _vergrendeld;
}

} // namespace domain
