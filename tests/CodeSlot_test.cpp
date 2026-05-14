/**
 * @file CodeSlot_test.cpp
 * @brief Unit-tests voor domain::CodeSlot (Strategy pattern - één van
 *        de Slot-implementaties).
 */

#include "domain/CodeSlot.h"

#include <gtest/gtest.h>

using domain::CodeSlot;

TEST(CodeSlot, StartVergrendeld)
{
    CodeSlot slot(1234);
    EXPECT_TRUE(slot.isVergrendeld());
}

TEST(CodeSlot, OntgrendelMetJuisteCodeAlsStringOpent)
{
    CodeSlot slot(1234);

    slot.ontgrendel("1234");

    EXPECT_FALSE(slot.isVergrendeld());
}

TEST(CodeSlot, OntgrendelMetFouteCodeBlijftVergrendeld)
{
    CodeSlot slot(1234);

    slot.ontgrendel("9999");

    EXPECT_TRUE(slot.isVergrendeld());
}

TEST(CodeSlot, OntgrendelMetNietNumeriekeStringBlijftVergrendeld)
{
    CodeSlot slot(1234);

    slot.ontgrendel("abc");

    EXPECT_TRUE(slot.isVergrendeld());
}

TEST(CodeSlot, VergrendelZetTerugOpVergrendeld)
{
    CodeSlot slot(1234);
    slot.ontgrendel("1234");
    ASSERT_FALSE(slot.isVergrendeld()) << "voorwaarde: eerst ontgrendelen";

    slot.vergrendel();

    EXPECT_TRUE(slot.isVergrendeld());
}
