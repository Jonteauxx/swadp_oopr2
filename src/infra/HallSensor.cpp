/**
 * @file HallSensor.cpp
 * @brief Implementatie van HallSensor (Infrastructure Layer).
 */

#include "HallSensor.h"

namespace infra {

void HallSensor::activeer()
{
    _geactiveerd = true;
}

void HallSensor::deactiveer()
{
    _geactiveerd = false;
}

bool HallSensor::isGeactiveerd() const
{
    return _geactiveerd;
}

} // namespace infra
