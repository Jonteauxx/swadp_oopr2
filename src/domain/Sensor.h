/**
 * @file Sensor.h
 * @brief Abstracte basisklasse voor sensoren in het gebouw (UML opdracht 1).
 * @author tj.herdigein
 * @date 2026
 */

#ifndef SWADP_OOPR2_SENSOR_H
#define SWADP_OOPR2_SENSOR_H

#include <utility>

// Forward declaration zodat de header niet aan Qt vastzit;
// alleen de .cpp en de tekenen-code includeren <QPaintDevice>.
class QPaintDevice;

namespace domain {

/**
 * @class Sensor
 * @brief Basisklasse voor sensoren met een positie en aan/uit-staat.
 *
 * Concept volgens het docent-diagram (opdracht1_4.png):
 *   - houdt een (x, y)-positie bij;
 *   - kan geactiveerd / gedeactiveerd worden;
 *   - kan zichzelf tekenen op een QPaintDevice (subklasse-specifiek).
 *
 * Subklasse-voorbeeld: HallSensor.
 */
class Sensor
{
public:
    /**
     * @brief Construeer een sensor op een gegeven positie.
     * @param x X-coordinaat in het venster (pixels).
     * @param y Y-coordinaat in het venster (pixels).
     */
    Sensor(int x, int y);

    virtual ~Sensor() = default;

    /// @brief Zet de sensor in geactiveerde staat.
    void activeer();

    /// @brief Zet de sensor in niet-geactiveerde staat.
    void deactiveer();

    /**
     * @brief Lees de huidige staat.
     * @return true als geactiveerd, anders false.
     */
    bool isGeactiveerd() const;

    /**
     * @brief Geef de positie van de sensor terug.
     * @return std::pair met (x, y).
     */
    std::pair<int, int> coordinaten() const;

    /**
     * @brief Teken de sensor op het opgegeven QPaintDevice.
     *
     * Pure virtual - subklassen bepalen het visuele uiterlijk
     * (bv. cirkel, kleur).
     *
     * @param target Qt paint-device (bv. een MainWindow).
     */
    virtual void teken(QPaintDevice* target) = 0;

protected:
    int  _x;            ///< x-coordinaat in pixels
    int  _y;            ///< y-coordinaat in pixels
    bool _geactiveerd;  ///< huidige staat
};

} // namespace domain

#endif // SWADP_OOPR2_SENSOR_H
