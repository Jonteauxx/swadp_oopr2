/**
 * @file Sensor_test.cpp
 * @brief Unit-tests voor domain::Sensor (Google Test).
 *
 * We testen alleen de NIET-tekenen-methodes; teken() vereist Qt en wordt
 * visueel geverifieerd via de draaiende applicatie.
 */

#include "domain/Sensor.h"

#include <gtest/gtest.h>

namespace {

/// Test-subklasse: trivial stub voor teken() zodat we Sensor kunnen instantieren
/// zonder Qt in de testbuild te halen.
class TestSensor : public domain::Sensor
{
public:
    using domain::Sensor::Sensor;
    void teken(QPaintDevice* /*target*/) override { /* no-op in tests */ }
};

} // anonymous namespace

TEST(Sensor, StartStaatIsNietGeactiveerd)
{
    TestSensor sensor(100, 200);
    EXPECT_FALSE(sensor.isGeactiveerd());
}

TEST(Sensor, ActiverenZetStaatOpTrue)
{
    TestSensor sensor(0, 0);

    sensor.activeer();
    EXPECT_TRUE(sensor.isGeactiveerd());
}

TEST(Sensor, DeactiverenZetStaatOpFalse)
{
    TestSensor sensor(0, 0);
    sensor.activeer();

    sensor.deactiveer();
    EXPECT_FALSE(sensor.isGeactiveerd());
}

TEST(Sensor, CoordinatenGeeftConstructorWaardenTerug)
{
    TestSensor sensor(515, 160);

    auto [x, y] = sensor.coordinaten();
    EXPECT_EQ(x, 515);
    EXPECT_EQ(y, 160);
}

TEST(Sensor, MeerdereActiverenIsIdempotent)
{
    TestSensor sensor(0, 0);
    sensor.activeer();
    sensor.activeer();
    sensor.activeer();

    EXPECT_TRUE(sensor.isGeactiveerd());
}
