/**
 * @file PigpioGpio.cpp
 * @brief Implementatie van PigpioGpio via pigpiod_if2.
 *
 * Wordt alleen volledig gecompileerd als HAS_PIGPIO is gedefinieerd
 * (door CMake gezet als de pigpiod_if2-library gevonden wordt). Op
 * Windows of zonder pigpio compileert deze file met stub-bodies die
 * altijd PigpioFout gooien — zo blijft de hele codebase compileer-baar
 * op meerdere platformen, maar pigpio-gebruik op niet-Pi geeft direct
 * een duidelijke fout.
 */

#include "PigpioGpio.h"

#ifdef HAS_PIGPIO
#include <pigpiod_if2.h>
#endif

#include <algorithm>
#include <sstream>

namespace infra {

namespace {

/**
 * @brief Vertaal een hoek 0..180 graden naar SG90 pulse-breedte in microseconden.
 *
 * - 0 graden  -> 500 us
 * - 90 graden -> 1500 us
 * - 180 graden -> 2500 us
 */
int hoekNaarPulsewidth(int hoekGraden)
{
    hoekGraden = std::clamp(hoekGraden, 0, 180);
    return 500 + (hoekGraden * 2000) / 180;
}

} // anonymous namespace

#ifdef HAS_PIGPIO
// =============================================================================
// Echte implementatie op de Raspberry Pi
// =============================================================================

PigpioGpio::PigpioGpio()
    : _handle(pigpio_start(nullptr, nullptr))
{
    if (_handle < 0) {
        std::ostringstream os;
        os << "Kan geen verbinding maken met pigpiod (foutcode " << _handle
           << "). Controleer: sudo systemctl status pigpiod";
        throw PigpioFout(os.str());
    }
}

PigpioGpio::~PigpioGpio()
{
    if (_handle >= 0) {
        pigpio_stop(_handle);
    }
}

void PigpioGpio::configureerOutput(int bcmPin)
{
    int rc = set_mode(_handle, bcmPin, PI_OUTPUT);
    if (rc < 0) {
        std::ostringstream os;
        os << "set_mode OUTPUT faalde voor pin " << bcmPin << " (rc=" << rc << ")";
        throw PigpioFout(os.str());
    }
}

void PigpioGpio::configureerInputPullup(int bcmPin)
{
    int rc = set_mode(_handle, bcmPin, PI_INPUT);
    if (rc < 0) {
        std::ostringstream os;
        os << "set_mode INPUT faalde voor pin " << bcmPin << " (rc=" << rc << ")";
        throw PigpioFout(os.str());
    }
    rc = set_pull_up_down(_handle, bcmPin, PI_PUD_UP);
    if (rc < 0) {
        std::ostringstream os;
        os << "set_pull_up_down PUD_UP faalde voor pin " << bcmPin
           << " (rc=" << rc << ")";
        throw PigpioFout(os.str());
    }
}

void PigpioGpio::schrijfDigitaal(int bcmPin, bool hoog)
{
    int rc = gpio_write(_handle, bcmPin, hoog ? 1 : 0);
    if (rc < 0) {
        std::ostringstream os;
        os << "gpio_write faalde voor pin " << bcmPin << " (rc=" << rc << ")";
        throw PigpioFout(os.str());
    }
}

bool PigpioGpio::leesDigitaal(int bcmPin)
{
    int waarde = gpio_read(_handle, bcmPin);
    if (waarde < 0) {
        std::ostringstream os;
        os << "gpio_read faalde voor pin " << bcmPin << " (rc=" << waarde << ")";
        throw PigpioFout(os.str());
    }
    return waarde == 1;
}

void PigpioGpio::zetServoHoek(int bcmPin, int hoekGraden)
{
    int pulse = hoekNaarPulsewidth(hoekGraden);
    int rc = set_servo_pulsewidth(_handle, bcmPin, pulse);
    if (rc < 0) {
        std::ostringstream os;
        os << "set_servo_pulsewidth faalde voor pin " << bcmPin
           << " (rc=" << rc << ")";
        throw PigpioFout(os.str());
    }
}

#else
// =============================================================================
// Stub-implementatie (compileren op Windows of zonder pigpio-library)
// =============================================================================

PigpioGpio::PigpioGpio() : _handle(-1)
{
    throw PigpioFout("pigpio is niet beschikbaar - "
                     "compileer op de Raspberry Pi met libpigpiod-if2.");
}

PigpioGpio::~PigpioGpio() = default;

void PigpioGpio::configureerOutput(int)      {}
void PigpioGpio::configureerInputPullup(int) {}
void PigpioGpio::schrijfDigitaal(int, bool)  {}
bool PigpioGpio::leesDigitaal(int)           { return false; }
void PigpioGpio::zetServoHoek(int, int)      {}

#endif

} // namespace infra
