/**
 * @file CodeSlot.cpp
 * @brief STUB-implementatie van CodeSlot.
 *        Tests zullen FAILEN met deze versie (RED-fase TDD).
 */

#include "CodeSlot.h"

namespace domain {

CodeSlot::CodeSlot(int code)
    : _code(code)
    , _vergrendeld(true)   // start vergrendeld - dat IS correct gedrag
{
}

void CodeSlot::ontgrendel(const std::string& /*eenCode*/)
{
    // STUB: hoort _vergrendeld = false te zetten als stoi(eenCode) == _code.
}

void CodeSlot::vergrendel()
{
    // STUB: hoort _vergrendeld = true te zetten.
}

bool CodeSlot::isVergrendeld() const
{
    return _vergrendeld;
}

} // namespace domain
