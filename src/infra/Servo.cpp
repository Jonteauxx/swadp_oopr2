/**
 * @file Servo.cpp
 * @brief STUB-implementatie van Servo - tests zullen FAILEN met deze versie.
 *        Echte logica volgt in de GREEN-fase van TDD.
 */

#include "Servo.h"

namespace infra {

Servo::Servo(IGpio& gpio, int bcmPin, int hoekDicht, int hoekOpen)
    : _gpio(gpio)
    , _bcmPin(bcmPin)
    , _hoekDicht(hoekDicht)
    , _hoekOpen(hoekOpen)
    , _huidigeHoek(-1)   // sentinel: nog niets aangestuurd
{
    // STUB: hoort naar dichte hoek te bewegen, doet nog niets.
}

void Servo::zetOpen()
{
    // STUB: hoort _huidigeHoek = _hoekOpen + _gpio.zetServoHoek(...) te doen.
}

void Servo::zetDicht()
{
    // STUB: hoort _huidigeHoek = _hoekDicht + _gpio.zetServoHoek(...) te doen.
}

int Servo::huidigeHoek() const
{
    return _huidigeHoek;
}

} // namespace infra
