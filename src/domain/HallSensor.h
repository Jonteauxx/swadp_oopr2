/**
 * @file HallSensor.h
 * @brief Hall-sensor voor de schuifdeur (detecteert magneet bij gesloten deur).
 * @author tj.herdigein
 * @date 2026
 */

#ifndef SWADP_OOPR2_HALLSENSOR_H
#define SWADP_OOPR2_HALLSENSOR_H

#include "Sensor.h"

namespace domain {

/**
 * @class HallSensor
 * @brief Concrete sensor die zich als gele cirkel (rust) of blauwe cirkel
 *        (geactiveerd) tekent.
 *
 * In het gebouw van firma L&B zit de hall-sensor naast de schuifdeur.
 * Wanneer de deur dicht is drukt hij tegen een magneet-plaat en wordt
 * de sensor geactiveerd. De schuifdeur weigert te sluiten als de
 * sensor al actief is (zie Schuifdeur::sluit).
 */
class HallSensor : public Sensor
{
public:
    /**
     * @brief Construeer een halsensor op een gegeven venster-positie.
     * @param x X-coordinaat (pixels).
     * @param y Y-coordinaat (pixels).
     */
    HallSensor(int x, int y);

    /**
     * @brief Teken een cirkel: geel als niet-geactiveerd, blauw als geactiveerd.
     * @param target Qt paint-device.
     */
    void teken(QPaintDevice* target) override;
};

} // namespace domain

#endif // SWADP_OOPR2_HALLSENSOR_H
