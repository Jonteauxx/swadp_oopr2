/**
 * @file PigpioGpio.h
 * @brief Concrete IGpio-implementatie via de pigpio-daemon (pigpiod_if2).
 * @author tj.herdigein
 * @date 2026
 */

#ifndef SWADP_OOPR2_PIGPIOGPIO_H
#define SWADP_OOPR2_PIGPIOGPIO_H

#include "IGpio.h"

#include <stdexcept>
#include <string>

namespace infra {

/**
 * @class PigpioFout
 * @brief Wordt gegooid bij pigpio-API-fouten of als de daemon niet draait.
 */
class PigpioFout : public std::runtime_error
{
public:
    explicit PigpioFout(const std::string& boodschap)
        : std::runtime_error(boodschap) {}
};

/**
 * @class PigpioGpio
 * @brief Echte IGpio-implementatie op de Raspberry Pi via pigpiod.
 *
 * Praat met de pigpio-daemon over een socket — dat betekent dat de
 * applicatie niet als root hoeft te draaien (in tegenstelling tot
 * de directe pigpio-library). De daemon moet wel actief zijn:
 * @code
 *   sudo systemctl start pigpiod
 * @endcode
 *
 * Werpt PigpioFout bij init-falen of bij pigpio API-fouten.
 */
class PigpioGpio : public IGpio
{
public:
    /**
     * @brief Maakt verbinding met de lokale pigpio-daemon.
     * @throws PigpioFout als pigpiod niet bereikbaar is.
     */
    PigpioGpio();

    /// @brief Sluit de verbinding met de daemon.
    ~PigpioGpio() override;

    // Een hardware-handle is uniek - kopieren mag niet.
    PigpioGpio(const PigpioGpio&)            = delete;
    PigpioGpio& operator=(const PigpioGpio&) = delete;

    // IGpio interface
    void configureerOutput(int bcmPin) override;
    void configureerInputPullup(int bcmPin) override;
    void schrijfDigitaal(int bcmPin, bool hoog) override;
    bool leesDigitaal(int bcmPin) override;
    void zetServoHoek(int bcmPin, int hoekGraden) override;

private:
    int _handle; ///< pigpio daemon handle (>=0 als geldig).
};

} // namespace infra

#endif // SWADP_OOPR2_PIGPIOGPIO_H
