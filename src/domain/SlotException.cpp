/**
 * @file SlotException.cpp
 * @brief Implementatie van SlotException (GREEN-fase TDD, opdracht 6).
 */

#include "SlotException.h"

namespace domain {

SlotException::SlotException(const std::string& plaats, const std::string& id)
    : _plaats(plaats)
    , _id(id)
    , _bericht("SlotException @ plaats '" + plaats + "' - id: " + id)
{
}

const std::string& SlotException::plaats() const noexcept
{
    return _plaats;
}

const std::string& SlotException::id() const noexcept
{
    return _id;
}

const char* SlotException::what() const noexcept
{
    return _bericht.c_str();
}

} // namespace domain
