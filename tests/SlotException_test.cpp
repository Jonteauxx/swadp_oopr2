/**
 * @file SlotException_test.cpp
 * @brief Unit-tests voor SlotException (opdracht 6).
 *
 * Toetst dat de exception zijn plaats + id bewaart, dat what() een
 * leesbare gecombineerde melding geeft, en dat hij als std::exception
 * opgevangen kan worden.
 */

#include "domain/SlotException.h"

#include <gtest/gtest.h>

#include <exception>
#include <string>

using domain::SlotException;

TEST(SlotExceptionTest, BewaartPlaatsEnId)
{
    SlotException ex("vd", "K001");

    EXPECT_EQ(ex.plaatsVanHetSlot(), "vd");
    EXPECT_EQ(ex.kaartVanBinnendringer(), "K001");
}

TEST(SlotExceptionTest, WhatBevatPlaatsEnId)
{
    SlotException ex("centraleHal", "geen idkaart voor XYZ");

    const std::string bericht = ex.what();

    EXPECT_NE(bericht.find("centraleHal"), std::string::npos);
    EXPECT_NE(bericht.find("geen idkaart voor XYZ"), std::string::npos);
}

TEST(SlotExceptionTest, IsOpvangbaarAlsStdException)
{
    try {
        throw SlotException("vd", "K001");
    }
    catch (const std::exception& e) {
        EXPECT_STRNE(e.what(), ""); // niet-lege melding
        SUCCEED();
        return;
    }
    FAIL() << "SlotException moet als std::exception opgevangen kunnen worden";
}
