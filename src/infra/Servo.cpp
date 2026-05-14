/**
 * @file Servo.cpp
 * @brief Implementatie van Servo (GREEN fase TDD).
 */

#include "Servo.h"

namespace infra {

Servo::Servo(IGpio& gpio, int bcmPin, int hoekDicht, int hoekOpen)
    : _gpio(gpio)
    , _bcmPin(bcmPin)
    , _hoekDicht(hoekDicht)
    , _hoekOpen(hoekOpen)
    , _huidigeHoek(hoekDicht)
{
    // Stuur de fysieke servo bij constructie meteen naar de dichte stand,
    // zodat logische- en fysieke staat synchroon zijn.
    _gpio.zetServoHoek(_bcmPin, _hoekDicht);
}

void Servo::zetOpen()
{
    _huidigeHoek = _hoekOpen;
    _gpio.zetServoHoek(_bcmPin, _hoekOpen);
}

void Servo::zetDicht()
{
    _huidigeHoek = _hoekDicht;
    _gpio.zetServoHoek(_bcmPin, _hoekDicht);
}

int Servo::huidigeHoek() const
{
    return _huidigeHoek;
}

} // namespace infra
