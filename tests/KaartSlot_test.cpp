/**
 * @file KaartSlot_test.cpp
 * @brief Unit-tests voor KaartSlot - inclusief de statische idKaarten-map
 *        en het zoeken met lambda calculus.
 *
 * @note KaartSlot heeft een statische map - we resetten die vóór elke
 *       test om geen onbedoelde state-leak tussen testcases te krijgen.
 */

#include "domain/IdKaart.h"
#include "domain/KaartSlot.h"

#include <gtest/gtest.h>

#include <algorithm>

using domain::IdKaart;
using domain::KaartSlot;

class KaartSlotTest : public ::testing::Test
{
protected:
    void SetUp() override { KaartSlot::wisAlleIdKaarten(); }
};

// =============================================================================
// Basis-gedrag
// =============================================================================

TEST_F(KaartSlotTest, StartVergrendeld)
{
    KaartSlot slot("vd");
    EXPECT_TRUE(slot.isVergrendeld());
}

TEST_F(KaartSlotTest, PlaatsGeefConstructorWaardeTerug)
{
    KaartSlot slot("centraleHal");
    EXPECT_EQ(slot.plaats(), "centraleHal");
}

TEST_F(KaartSlotTest, OntgrendelMetOnbekendeIdBlijftVergrendeld)
{
    KaartSlot slot("vd");

    slot.ontgrendel("OnbekendeKaart");

    EXPECT_TRUE(slot.isVergrendeld());
}

TEST_F(KaartSlotTest, OntgrendelMetGeregistreerdeKaartZonderToegangBlijftVergrendeld)
{
    KaartSlot slot("vd");
    IdKaart kaart("K001", "Anna", "Adres");
    KaartSlot::voegIdKaartToe(&kaart);
    // Kaart is GEREGISTREERD maar heeft GEEN toegang tot dit slot.

    slot.ontgrendel("K001");

    EXPECT_TRUE(slot.isVergrendeld());
}

TEST_F(KaartSlotTest, OntgrendelMetGeautoriseerdeKaartOpent)
{
    KaartSlot slot("vd");
    IdKaart kaart("K001", "Anna", "Adres");
    kaart.geefToegang(&slot);
    KaartSlot::voegIdKaartToe(&kaart);

    slot.ontgrendel("K001");

    EXPECT_FALSE(slot.isVergrendeld());
}

TEST_F(KaartSlotTest, VergrendelZetTerug)
{
    KaartSlot slot("vd");
    IdKaart kaart("K001", "Anna", "Adres");
    kaart.geefToegang(&slot);
    KaartSlot::voegIdKaartToe(&kaart);
    slot.ontgrendel("K001");
    ASSERT_FALSE(slot.isVergrendeld());

    slot.vergrendel();

    EXPECT_TRUE(slot.isVergrendeld());
}

// =============================================================================
// Statische map - voegIdKaartToe / verwijderIdKaart / aantalIdKaarten
// =============================================================================

TEST_F(KaartSlotTest, VoegIdKaartToeRegistreertInMap)
{
    IdKaart kaart("K001", "Anna", "Adres");

    KaartSlot::voegIdKaartToe(&kaart);

    EXPECT_EQ(KaartSlot::aantalIdKaarten(), 1u);
}

TEST_F(KaartSlotTest, VerwijderIdKaartHaaltUitMap)
{
    IdKaart kaart("K001", "Anna", "Adres");
    KaartSlot::voegIdKaartToe(&kaart);

    KaartSlot::verwijderIdKaart("K001");

    EXPECT_EQ(KaartSlot::aantalIdKaarten(), 0u);
}

TEST_F(KaartSlotTest, MapWordtGedeeldTussenAlleKaartSlotInstanties)
{
    KaartSlot slotA("vd");
    KaartSlot slotB("d1");
    IdKaart kaart("K001", "Anna", "Adres");
    KaartSlot::voegIdKaartToe(&kaart);

    // Beide slot-instanties zien de kaart, want de map is statisch.
    EXPECT_EQ(KaartSlot::aantalIdKaarten(), 1u);
    (void)slotA; (void)slotB; // suppress unused-warning
}

// =============================================================================
// Lambda calculus - zoeken in de statische map (remedial-eis)
// =============================================================================

TEST_F(KaartSlotTest, ZoekIdKaartenMetLambdaOpNaam)
{
    IdKaart anna1("K001", "Anna", "Adres 1");
    IdKaart anna2("K002", "Anna", "Adres 2");
    IdKaart bob   ("K003", "Bob",  "Adres 3");
    KaartSlot::voegIdKaartToe(&anna1);
    KaartSlot::voegIdKaartToe(&anna2);
    KaartSlot::voegIdKaartToe(&bob);

    auto annas = KaartSlot::zoekIdKaarten(
        [](const IdKaart& k) { return k.naamEigenaar() == "Anna"; });

    EXPECT_EQ(annas.size(), 2u);
}

TEST_F(KaartSlotTest, ZoekIdKaartenMetLambdaOpAdres)
{
    IdKaart anna("K001", "Anna", "Hoofdstraat 1");
    IdKaart bob  ("K002", "Bob",  "Hoofdstraat 2");
    IdKaart carla("K003", "Carla","Kerklaan 5");
    KaartSlot::voegIdKaartToe(&anna);
    KaartSlot::voegIdKaartToe(&bob);
    KaartSlot::voegIdKaartToe(&carla);

    auto hoofdstraat = KaartSlot::zoekIdKaarten(
        [](const IdKaart& k) {
            return k.adresEigenaar().find("Hoofdstraat") != std::string::npos;
        });

    EXPECT_EQ(hoofdstraat.size(), 2u);
}
