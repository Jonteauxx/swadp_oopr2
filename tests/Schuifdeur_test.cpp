/**
 * @file Schuifdeur_test.cpp
 * @brief Unit-tests voor domain::Schuifdeur - vooral de sensor-controle bij sluit().
 */

#include "domain/Schuifdeur.h"
#include "domain/Sensor.h"

#include <gtest/gtest.h>

namespace {

/// Test-sensor: minimale implementatie van de pure interface.
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

TEST(Schuifdeur, SluitNormaalAlsSensorNietGeactiveerd)
{
    TestSensor sensor;
    domain::Schuifdeur deur(100, 100, 60, &sensor);

    deur.open();
    deur.sluit();
    EXPECT_FALSE(deur.isDeurOpen());
}

TEST(Schuifdeur, SluitGenegeerdAlsSensorActief)
{
    TestSensor sensor;
    sensor.activeer();
    domain::Schuifdeur deur(100, 100, 60, &sensor);

    deur.open();
    deur.sluit();
    EXPECT_TRUE(deur.isDeurOpen()) << "Schuifdeur moet open blijven bij actieve hal-sensor";
}

TEST(Schuifdeur, SluitWerktAlsSensorNullIs)
{
    domain::Schuifdeur deur(100, 100, 60, nullptr);

    deur.open();
    deur.sluit();
    EXPECT_FALSE(deur.isDeurOpen());
}

TEST(Schuifdeur, SluitWerktNadatSensorWeerGedeactiveerd)
{
    TestSensor sensor;
    domain::Schuifdeur deur(100, 100, 60, &sensor);

    deur.open();
    sensor.activeer();
    deur.sluit();              // genegeerd
    EXPECT_TRUE(deur.isDeurOpen());

    sensor.deactiveer();
    deur.sluit();              // mag nu wel
    EXPECT_FALSE(deur.isDeurOpen());
}
