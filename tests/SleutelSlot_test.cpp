/**
 * @file SleutelSlot_test.cpp
 * @brief Unit-tests voor domain::SleutelSlot (Strategy pattern - één van
 *        de Slot-implementaties).
 */

#include "domain/SleutelSlot.h"

#include <gtest/gtest.h>

using domain::SleutelSlot;

TEST(SleutelSlot, StartVergrendeld)
{
    SleutelSlot slot("geheim");
    EXPECT_TRUE(slot.isVergrendeld());
}

TEST(SleutelSlot, OntgrendelMetJuisteSleutelOpent)
{
    SleutelSlot slot("geheim");

    slot.ontgrendel("geheim");

    EXPECT_FALSE(slot.isVergrendeld());
}

TEST(SleutelSlot, OntgrendelMetFouteSleutelBlijftVergrendeld)
{
    SleutelSlot slot("geheim");

    slot.ontgrendel("verkeerd");

    EXPECT_TRUE(slot.isVergrendeld());
}

TEST(SleutelSlot, VergrendelZetTerugOpVergrendeld)
{
    SleutelSlot slot("geheim");
    slot.ontgrendel("geheim");
    ASSERT_FALSE(slot.isVergrendeld()) << "voorwaarde: eerst ontgrendelen";

    slot.vergrendel();

    EXPECT_TRUE(slot.isVergrendeld());
}

TEST(SleutelSlot, OntgrendelEnVergrendelMeerdereKeer)
{
    SleutelSlot slot("geheim");
    slot.ontgrendel("geheim");
    slot.vergrendel();
    slot.ontgrendel("geheim");

    EXPECT_FALSE(slot.isVergrendeld());
}
