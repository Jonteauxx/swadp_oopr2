/**
 * @file Sensor_test.cpp
 * @brief Unit-tests voor domain::Sensor interface + infra::HallSensor.
 */

#include "domain/Sensor.h"
#include "infra/HallSensor.h"

#include <gtest/gtest.h>

namespace {

/// Test-subklasse: minimale stub die de pure interface implementeert.
class TestSensor : public domain::Sensor
{
public:
    void activeer() override       { _geactiveerd = true; }
    void deactiveer() override     { _geactiveerd = false; }
    bool isGeactiveerd() const override { return _geactiveerd; }
private:
    bool _geactiveerd = false;
};

} // anonymous namespace

TEST(Sensor, StartStaatIsNietGeactiveerd)
{
    TestSensor sensor;
    EXPECT_FALSE(sensor.isGeactiveerd());
}

TEST(Sensor, ActiverenZetStaatOpTrue)
{
    TestSensor sensor;

    sensor.activeer();
    EXPECT_TRUE(sensor.isGeactiveerd());
}

TEST(Sensor, DeactiverenZetStaatOpFalse)
{
    TestSensor sensor;
    sensor.activeer();

    sensor.deactiveer();
    EXPECT_FALSE(sensor.isGeactiveerd());
}

TEST(Sensor, MeerdereActiverenIsIdempotent)
{
    TestSensor sensor;
    sensor.activeer();
    sensor.activeer();
    sensor.activeer();

    EXPECT_TRUE(sensor.isGeactiveerd());
}

// --- HallSensor (infra) directe tests ---

TEST(HallSensor, StartStaatIsNietGeactiveerd)
{
    infra::HallSensor hs;
    EXPECT_FALSE(hs.isGeactiveerd());
}

TEST(HallSensor, ActiverenEnDeactiveren)
{
    infra::HallSensor hs;
    hs.activeer();
    EXPECT_TRUE(hs.isGeactiveerd());

    hs.deactiveer();
    EXPECT_FALSE(hs.isGeactiveerd());
}
