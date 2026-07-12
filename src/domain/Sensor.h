/**
 * @file Sensor.h
 * @brief Pure interface voor sensoren in het gebouw.
 * @author tj.herdigein
 * @date 2026
 */

#ifndef SWADP_OOPR2_SENSOR_H
#define SWADP_OOPR2_SENSOR_H

namespace domain {

/**
 * @class Sensor
 * @brief Pure interface voor sensoren (DIP — domein definieert contract,
 *        infra levert implementatie).
 *
 * Concrete sensoren (HallSensor, IR, ultrasoon …) leven in de
 * Infrastructure Layer en implementeren deze interface.
 */
class Sensor
{
public:
    virtual ~Sensor() = default;

    /// @brief Zet de sensor in geactiveerde staat.
    virtual void activeer() = 0;

    /// @brief Zet de sensor in niet-geactiveerde staat.
    virtual void deactiveer() = 0;

    /**
     * @brief Lees de huidige staat.
     * @return true als geactiveerd, anders false.
     */
    virtual bool isGeactiveerd() const = 0;
};

} // namespace domain

#endif // SWADP_OOPR2_SENSOR_H
