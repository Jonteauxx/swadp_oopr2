/**
 * @file MockGpio_test.cpp
 * @brief Unit-tests voor MockGpio (Google Test).
 *
 * Verifieert dat de mock-implementatie zich gedraagt zoals de
 * IGpio-interface belooft. Belangrijk: als deze tests slagen, kunnen
 * we MockGpio veilig inzetten in domain-tests waar IGpio nodig is.
 */

#include "MockGpio.h"

#include <gtest/gtest.h>

using infra::MockGpio;

// -----------------------------------------------------------------------------
// Output
// -----------------------------------------------------------------------------

TEST(MockGpio, GeschrevenWaardeIsTerugleesbaar)
{
    MockGpio gpio;
    gpio.configureerOutput(25);

    gpio.schrijfDigitaal(25, true);
    EXPECT_TRUE(gpio.laatsteGeschrevenWaarde(25));

    gpio.schrijfDigitaal(25, false);
    EXPECT_FALSE(gpio.laatsteGeschrevenWaarde(25));
}

// -----------------------------------------------------------------------------
// Input + pull-up
// -----------------------------------------------------------------------------

TEST(MockGpio, PullupInputStaatStandaardHoog)
{
    MockGpio gpio;
    gpio.configureerInputPullup(17);

    EXPECT_TRUE(gpio.leesDigitaal(17));
}

TEST(MockGpio, KnopdrukMaaktPinLaag)
{
    MockGpio gpio;
    gpio.configureerInputPullup(17);

    gpio.simuleerKnopdruk(17);
    EXPECT_FALSE(gpio.leesDigitaal(17));
}

TEST(MockGpio, KnopLoslatenMaaktPinWeerHoog)
{
    MockGpio gpio;
    gpio.configureerInputPullup(17);
    gpio.simuleerKnopdruk(17);

    gpio.simuleerKnopLoslaten(17);
    EXPECT_TRUE(gpio.leesDigitaal(17));
}

// -----------------------------------------------------------------------------
// Servo
// -----------------------------------------------------------------------------

TEST(MockGpio, ServoHoekWordtBijgehouden)
{
    MockGpio gpio;
    gpio.zetServoHoek(18, 45);

    EXPECT_EQ(gpio.laatsteServoHoek(18), 45);
}

TEST(MockGpio, ServoHoekWordtGeclamptBinnenBereik)
{
    MockGpio gpio;

    gpio.zetServoHoek(18, -50);
    EXPECT_EQ(gpio.laatsteServoHoek(18), 0);

    gpio.zetServoHoek(18, 250);
    EXPECT_EQ(gpio.laatsteServoHoek(18), 180);
}

// -----------------------------------------------------------------------------
// Actie-log (handig voor debugging tijdens domain-tests)
// -----------------------------------------------------------------------------

TEST(MockGpio, ActieLogToontVolgordeVanAanroepen)
{
    MockGpio gpio;
    gpio.configureerOutput(25);
    gpio.schrijfDigitaal(25, true);
    gpio.schrijfDigitaal(25, false);

    const auto& log = gpio.acties();
    ASSERT_EQ(log.size(), 3u);
    EXPECT_EQ(log[0], "config_output 25");
    EXPECT_EQ(log[1], "write 25 1");
    EXPECT_EQ(log[2], "write 25 0");
}

TEST(MockGpio, ResetLogWistDeGeschiedenis)
{
    MockGpio gpio;
    gpio.configureerOutput(25);
    gpio.schrijfDigitaal(25, true);

    gpio.resetLog();
    EXPECT_TRUE(gpio.acties().empty());
}
