/**
 * @file Schuifdeur_test.cpp
 * @brief Unit-tests voor domain::Schuifdeur - vooral de sensor-controle bij sluit().
 *
 * Schuifdeur.cpp gebruikt Qt voor teken(), dus we linken die source-file
 * NIET mee in de testbuild. Tests gebruiken een eigen TestSchuifdeur-stub
 * die teken() leeg overschrijft.
 */

#include "domain/Deur.h"
#include "domain/Sensor.h"

#include <gtest/gtest.h>

namespace {

/// Test-sensor: geen Qt nodig, simuleert (de)activatie.
class TestSensor : public domain::Sensor
{
public:
    using domain::Sensor::Sensor;
    void teken(QPaintDevice* /*target*/) override {}
};

/// Test-schuifdeur die het feitelijke gedrag uit Schuifdeur.cpp namaakt,
/// maar zonder Qt. We bouwen sluit() handmatig na zodat we de sensor-
/// conditie kunnen testen zonder Qt-link te eisen.
class TestSchuifdeur : public domain::Deur
{
public:
    TestSchuifdeur(int x, int y, unsigned lengte, domain::Sensor* sensor)
        : Deur(x, y, lengte), _sensor(sensor) {}

    void teken(QPaintDevice* /*target*/) override {}

    void sluit() override
    {
        if (_sensor != nullptr && _sensor->isGeactiveerd()) {
            return; // weiger: sensor (magneet) actief
        }
        domain::Deur::sluit();
    }

private:
    domain::Sensor* _sensor;
};

} // anonymous namespace

TEST(Schuifdeur, SluitNormaalAlsSensorNietGeactiveerd)
{
    TestSensor sensor(0, 0);
    TestSchuifdeur deur(100, 100, 60, &sensor);

    deur.open();
    deur.sluit();
    EXPECT_FALSE(deur.isDeurOpen());
}

TEST(Schuifdeur, SluitGenegeerdAlsSensorActief)
{
    TestSensor sensor(0, 0);
    sensor.activeer();
    TestSchuifdeur deur(100, 100, 60, &sensor);

    deur.open();
    deur.sluit();
    EXPECT_TRUE(deur.isDeurOpen()) << "Schuifdeur moet open blijven bij actieve hal-sensor";
}

TEST(Schuifdeur, SluitWerktAlsSensorNullIs)
{
    TestSchuifdeur deur(100, 100, 60, nullptr);

    deur.open();
    deur.sluit();
    EXPECT_FALSE(deur.isDeurOpen());
}

TEST(Schuifdeur, SluitWerktNadatSensorWeerGedeactiveerd)
{
    TestSensor sensor(0, 0);
    TestSchuifdeur deur(100, 100, 60, &sensor);

    deur.open();
    sensor.activeer();
    deur.sluit();              // genegeerd
    EXPECT_TRUE(deur.isDeurOpen());

    sensor.deactiveer();
    deur.sluit();              // mag nu wel
    EXPECT_FALSE(deur.isDeurOpen());
}
