/**
 * @file MockGpio.h
 * @brief In-memory IGpio-implementatie voor unit-tests (geen Pi nodig).
 * @author tj.herdigein
 * @date 2026
 */

#ifndef SWADP_OOPR2_MOCKGPIO_H
#define SWADP_OOPR2_MOCKGPIO_H

#include "IGpio.h"

#include <map>
#include <string>
#include <vector>

namespace infra {

/**
 * @class MockGpio
 * @brief Fake-implementatie van IGpio die alle GPIO-acties in memory bijhoudt.
 *
 * Wordt gebruikt door unit-tests zodat we de Pi-hardware niet nodig hebben.
 * Naast de standaard IGpio-methodes biedt MockGpio extra test-hulpmiddelen:
 *   - Knopdrukken simuleren (simuleerKnopdruk).
 *   - Verifieren wat de code geschreven heeft (laatsteGeschrevenWaarde).
 *   - De volledige log van uitgevoerde acties inspecteren (acties).
 */
class MockGpio : public IGpio
{
public:
    MockGpio();

    // IGpio interface
    void configureerOutput(int bcmPin) override;
    void configureerInputPullup(int bcmPin) override;
    void schrijfDigitaal(int bcmPin, bool hoog) override;
    bool leesDigitaal(int bcmPin) override;
    void zetServoHoek(int bcmPin, int hoekGraden) override;

    // ------------------------------------------------------------------
    // Test-hulpmiddelen
    // ------------------------------------------------------------------

    /**
     * @brief Simuleer dat een fysieke knop wordt ingedrukt.
     *
     * Bij actieve pull-up trekt het indrukken de pin naar LAAG.
     * @param bcmPin BCM-nummer van de knop-pin.
     */
    void simuleerKnopdruk(int bcmPin);

    /// @brief Simuleer dat een fysieke knop wordt losgelaten (pin weer HOOG).
    void simuleerKnopLoslaten(int bcmPin);

    /**
     * @brief Lees terug wat als laatste naar een pin geschreven is.
     * @return false als er nog niks geschreven is, anders de laatste waarde.
     */
    bool laatsteGeschrevenWaarde(int bcmPin) const;

    /**
     * @brief Lees terug welke servo-hoek als laatste gezet is voor een pin.
     * @return -1 als er nog geen hoek gezet is, anders de hoek in graden.
     */
    int laatsteServoHoek(int bcmPin) const;

    /// @brief Volledige log van uitgevoerde acties (in volgorde).
    const std::vector<std::string>& acties() const { return _acties; }

    /// @brief Wis het actie-log (handig tussen test-stappen).
    void resetLog();

private:
    std::map<int, bool> _outputStaat;
    std::map<int, bool> _inputStaat;  ///< default: true (pull-up hoog)
    std::map<int, int>  _servoHoek;
    std::vector<std::string> _acties;
};

} // namespace infra

#endif // SWADP_OOPR2_MOCKGPIO_H
