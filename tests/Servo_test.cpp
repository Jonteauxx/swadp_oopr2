/**
 * @file Servo_test.cpp
 * @brief Unit-tests voor infra::Servo, gebruikmakend van MockGpio.
 *
 * Met deze tests testen we dat Servo:
 *   - bij constructie zichzelf naar de dichte hoek beweegt;
 *   - bij zetOpen() de open-hoek naar IGpio stuurt;
 *   - bij zetDicht() de dichte-hoek stuurt;
 *   - huidigeHoek() het laatste commando teruggeeft;
 *   - de hoeken uit de constructor respecteert (geen hardcoded 0/180).
 */

#include "infra/MockGpio.h"
#include "infra/Servo.h"

#include <gtest/gtest.h>

using infra::MockGpio;
using infra::Servo;

TEST(Servo, ConstructorZetServoOpDichteHoek)
{
    MockGpio gpio;
    Servo servo(gpio, 18, 0, 180);

    EXPECT_EQ(gpio.laatsteServoHoek(18), 0);
}

TEST(Servo, ZetOpenSchrijftOpenHoekNaarGpio)
{
    MockGpio gpio;
    Servo servo(gpio, 18, 0, 180);

    servo.zetOpen();

    EXPECT_EQ(gpio.laatsteServoHoek(18), 180);
}

TEST(Servo, ZetDichtSchrijftDichteHoekNaarGpio)
{
    MockGpio gpio;
    Servo servo(gpio, 18, 0, 180);

    servo.zetOpen();
    servo.zetDicht();

    EXPECT_EQ(gpio.laatsteServoHoek(18), 0);
}

TEST(Servo, HuidigeHoekVolgtLaatsteCommando)
{
    MockGpio gpio;
    Servo servo(gpio, 18, 0, 180);

    EXPECT_EQ(servo.huidigeHoek(), 0) << "Na constructie moet servo dicht zijn";

    servo.zetOpen();
    EXPECT_EQ(servo.huidigeHoek(), 180);

    servo.zetDicht();
    EXPECT_EQ(servo.huidigeHoek(), 0);
}

TEST(Servo, GebruiktAangepasteHoekenUitConstructor)
{
    MockGpio gpio;
    Servo servo(gpio, 23, 45, 135);

    EXPECT_EQ(gpio.laatsteServoHoek(23), 45)
        << "Constructor moet de meegegeven dicht-hoek gebruiken, niet 0";

    servo.zetOpen();
    EXPECT_EQ(gpio.laatsteServoHoek(23), 135);
    EXPECT_EQ(servo.huidigeHoek(), 135);
}
