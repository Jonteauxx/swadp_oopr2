/**
 * @file IGpio.h
 * @brief Hardware-abstractie voor GPIO-toegang (Facade pattern).
 * @author tj.herdigein
 * @date 2026
 */

#ifndef SWADP_OOPR2_IGPIO_H
#define SWADP_OOPR2_IGPIO_H

namespace infra {

/**
 * @class IGpio
 * @brief Abstracte interface voor GPIO-pin toegang op de Raspberry Pi.
 *
 * Verbergt de pigpio-library achter vijf methodes. Het domein praat
 * alleen tegen IGpio, nooit direct tegen pigpio. Dat levert op:
 *   - Unit-tests mogelijk zonder Pi (via MockGpio).
 *   - GPIO-library is uitwisselbaar zonder domain-code aan te raken.
 *   - Dependency Inversion Principle expliciet toegepast.
 *
 * @note Pin-nummers zijn altijd BCM-nummering (de "GPIO XX" labels),
 *       nooit fysieke pin-nummers. Zie docs/ voor de mapping.
 */
class IGpio
{
public:
    virtual ~IGpio() = default;

    /**
     * @brief Stel een pin in als digitale uitgang.
     * @param bcmPin BCM-nummer van de pin (bv. 25 voor de LED).
     */
    virtual void configureerOutput(int bcmPin) = 0;

    /**
     * @brief Stel een pin in als digitale ingang met interne pull-up.
     *
     * Met een actieve pull-up is de pin standaard HOOG; een ingedrukte
     * knop (verbonden met GND) trekt de pin naar LAAG.
     *
     * @param bcmPin BCM-nummer van de pin (bv. 17 voor knop schuifdeur).
     */
    virtual void configureerInputPullup(int bcmPin) = 0;

    /**
     * @brief Schrijf hoog (true) of laag (false) naar een output-pin.
     * @param bcmPin BCM-nummer.
     * @param hoog   true = HOOG (3.3V), false = LAAG (0V).
     */
    virtual void schrijfDigitaal(int bcmPin, bool hoog) = 0;

    /**
     * @brief Lees de huidige staat van een input-pin.
     * @param bcmPin BCM-nummer.
     * @return true als pin HOOG is, false als pin LAAG is.
     */
    virtual bool leesDigitaal(int bcmPin) = 0;

    /**
     * @brief Zet een servomotor op een gegeven hoek.
     *
     * Intern wordt de hoek vertaald naar een PWM-pulsbreedte volgens
     * de SG90-standaard: 0 graden = 500 us, 180 graden = 2500 us.
     *
     * @param bcmPin     BCM-nummer van de signal-pin.
     * @param hoekGraden Doelhoek 0..180; waarden buiten dat bereik
     *                   worden geclampt.
     */
    virtual void zetServoHoek(int bcmPin, int hoekGraden) = 0;
};

} // namespace infra

#endif // SWADP_OOPR2_IGPIO_H
