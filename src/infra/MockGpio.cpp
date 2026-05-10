/**
 * @file MockGpio.cpp
 * @brief Implementatie van de in-memory mock van IGpio.
 */

#include "MockGpio.h"

#include <algorithm>
#include <sstream>

namespace infra {

MockGpio::MockGpio() = default;

void MockGpio::configureerOutput(int bcmPin)
{
    _outputStaat[bcmPin] = false;
    std::ostringstream os;
    os << "config_output " << bcmPin;
    _acties.push_back(os.str());
}

void MockGpio::configureerInputPullup(int bcmPin)
{
    _inputStaat[bcmPin] = true; // pull-up: standaard HOOG
    std::ostringstream os;
    os << "config_input_pullup " << bcmPin;
    _acties.push_back(os.str());
}

void MockGpio::schrijfDigitaal(int bcmPin, bool hoog)
{
    _outputStaat[bcmPin] = hoog;
    std::ostringstream os;
    os << "write " << bcmPin << " " << (hoog ? "1" : "0");
    _acties.push_back(os.str());
}

bool MockGpio::leesDigitaal(int bcmPin)
{
    // Niet-geconfigureerde pin = HOOG (alsof pull-up actief is).
    auto it = _inputStaat.find(bcmPin);
    return it != _inputStaat.end() ? it->second : true;
}

void MockGpio::zetServoHoek(int bcmPin, int hoekGraden)
{
    hoekGraden = std::clamp(hoekGraden, 0, 180);
    _servoHoek[bcmPin] = hoekGraden;
    std::ostringstream os;
    os << "servo " << bcmPin << " " << hoekGraden;
    _acties.push_back(os.str());
}

void MockGpio::simuleerKnopdruk(int bcmPin)
{
    _inputStaat[bcmPin] = false; // ingedrukt = LAAG (verbinding met GND)
}

void MockGpio::simuleerKnopLoslaten(int bcmPin)
{
    _inputStaat[bcmPin] = true;
}

bool MockGpio::laatsteGeschrevenWaarde(int bcmPin) const
{
    auto it = _outputStaat.find(bcmPin);
    return it != _outputStaat.end() ? it->second : false;
}

int MockGpio::laatsteServoHoek(int bcmPin) const
{
    auto it = _servoHoek.find(bcmPin);
    return it != _servoHoek.end() ? it->second : -1;
}

void MockGpio::resetLog()
{
    _acties.clear();
}

} // namespace infra
