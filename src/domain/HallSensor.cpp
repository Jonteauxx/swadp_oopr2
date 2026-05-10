/**
 * @file HallSensor.cpp
 * @brief Implementatie van HallSensor.teken().
 */

#include "HallSensor.h"

#include <QBrush>
#include <QColor>
#include <QPaintDevice>
#include <QPainter>
#include <QPen>

namespace domain {

namespace {
    /// Diameter van de sensor-cirkel in pixels.
    constexpr int DIAMETER = 18;
}

HallSensor::HallSensor(int x, int y)
    : Sensor(x, y)
{
}

void HallSensor::teken(QPaintDevice* target)
{
    QPainter painter(target);

    const QColor kleur = _geactiveerd ? Qt::blue : Qt::yellow;

    painter.setBrush(QBrush(kleur, Qt::SolidPattern));
    painter.setPen(QPen(kleur, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawEllipse(_x, _y, DIAMETER, DIAMETER);
}

} // namespace domain
