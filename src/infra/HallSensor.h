/**
 * @file HallSensor.h
 * @brief Hall-sensor implementatie (Infrastructure Layer).
 * @author tj.herdigein
 * @date 2026
 */

#ifndef SWADP_OOPR2_HALLSENSOR_H
#define SWADP_OOPR2_HALLSENSOR_H

#include "domain/Sensor.h"

namespace infra {

/**
 * @class HallSensor
 * @brief Concrete sensor die een magnetisch veld detecteert.
 *
 * Implementeert de domain::Sensor interface. Bevat puur state-logica;
 * het tekenen van de sensor gebeurt in de presentatielaag (MainWindow).
 */
class HallSensor : public domain::Sensor
{
public:
    HallSensor() = default;

    void activeer() override;
    void deactiveer() override;
    bool isGeactiveerd() const override;

private:
    bool _geactiveerd = false;
};

} // namespace infra

#endif // SWADP_OOPR2_HALLSENSOR_H
