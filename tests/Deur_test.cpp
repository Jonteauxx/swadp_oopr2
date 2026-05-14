/**
 * @file Deur_test.cpp
 * @brief Unit-tests voor domain::Deur (basis-gedrag + slot-interactie,
 *        zowel één slot als meerdere sloten - opdracht 1, 2, 3).
 */

#include "domain/Deur.h"
#include "domain/SleutelSlot.h"

#include <gtest/gtest.h>

#include <memory>

namespace {

/// Test-subklasse: trivial stub voor teken() zodat we Deur kunnen instantieren
/// in tests zonder Qt te linken.
class TestDeur : public domain::Deur
{
public:
    using domain::Deur::Deur;
    void teken(QPaintDevice* /*target*/) override { /* no-op in tests */ }
};

} // anonymous namespace

// =============================================================================
// Basis-gedrag (opdracht 1, deur zonder sloten)
// =============================================================================

TEST(Deur, StartStaatIsDicht)
{
    TestDeur deur(100, 200, 50);
    EXPECT_FALSE(deur.isDeurOpen());
}

TEST(Deur, OpenZetStatusOpTrue)
{
    TestDeur deur(0, 0, 60);
    deur.open();
    EXPECT_TRUE(deur.isDeurOpen());
}

TEST(Deur, SluitZetStatusOpFalse)
{
    TestDeur deur(0, 0, 60);
    deur.open();
    deur.sluit();
    EXPECT_FALSE(deur.isDeurOpen());
}

TEST(Deur, DeurLengteGeeftConstructorWaardeTerug)
{
    TestDeur deur(0, 0, 75);
    EXPECT_EQ(deur.deurLengte(), 75u);
}

TEST(Deur, MeerdereKeerOpenIsIdempotent)
{
    TestDeur deur(0, 0, 60);
    deur.open();
    deur.open();
    deur.open();
    EXPECT_TRUE(deur.isDeurOpen());
}

// =============================================================================
// Eén slot per deur (opdracht 2)
// =============================================================================

TEST(DeurMetSlot, ZonderSlotOpenBlijftWerken)
{
    TestDeur deur(0, 0, 50);
    deur.open();
    EXPECT_TRUE(deur.isDeurOpen());
}

TEST(DeurMetSlot, OpenWeigertBijVergrendeldSlot)
{
    TestDeur deur(0, 0, 50);
    deur.voegSlotToe(std::make_shared<domain::SleutelSlot>("geheim"));

    deur.open();

    EXPECT_FALSE(deur.isDeurOpen())
        << "Een deur met vergrendeld slot mag niet open gaan";
}

TEST(DeurMetSlot, OpenWerktNaSlotOntgrendelen)
{
    TestDeur deur(0, 0, 50);
    auto slot = std::make_shared<domain::SleutelSlot>("geheim");
    deur.voegSlotToe(slot);

    slot->ontgrendel("geheim");
    deur.open();

    EXPECT_TRUE(deur.isDeurOpen());
}

TEST(DeurMetSlot, NaSlotOpnieuwVergrendelenGaatDeurNietMeerOpen)
{
    TestDeur deur(0, 0, 50);
    auto slot = std::make_shared<domain::SleutelSlot>("geheim");
    deur.voegSlotToe(slot);
    slot->ontgrendel("geheim");
    deur.open();
    deur.sluit();

    slot->vergrendel();
    deur.open();

    EXPECT_FALSE(deur.isDeurOpen());
}

TEST(DeurMetSlot, SluitVergrendeltSlotAutomatisch)
{
    TestDeur deur(0, 0, 50);
    auto slot = std::make_shared<domain::SleutelSlot>("geheim");
    deur.voegSlotToe(slot);
    slot->ontgrendel("geheim");
    deur.open();
    ASSERT_TRUE(deur.isDeurOpen());

    deur.sluit();

    EXPECT_TRUE(slot->isVergrendeld())
        << "Per opdracht 2: bij sluit() moet het slot automatisch vergrendeld worden";
}

// =============================================================================
// Meerdere sloten per deur (opdracht 3)
// =============================================================================

TEST(DeurMetSloten, OpenWeigertAlsEenSlotVergrendeld)
{
    TestDeur deur(0, 0, 50);
    auto slot1 = std::make_shared<domain::SleutelSlot>("A");
    auto slot2 = std::make_shared<domain::SleutelSlot>("B");
    deur.voegSlotToe(slot1);
    deur.voegSlotToe(slot2);

    slot1->ontgrendel("A");
    // slot2 nog vergrendeld
    deur.open();

    EXPECT_FALSE(deur.isDeurOpen())
        << "Met 1 vergrendeld slot mag de deur niet open";
}

TEST(DeurMetSloten, OpenWerktAlleenAlsAlleSlotenOntgrendeld)
{
    TestDeur deur(0, 0, 50);
    auto slot1 = std::make_shared<domain::SleutelSlot>("A");
    auto slot2 = std::make_shared<domain::SleutelSlot>("B");
    deur.voegSlotToe(slot1);
    deur.voegSlotToe(slot2);

    slot1->ontgrendel("A");
    slot2->ontgrendel("B");
    deur.open();

    EXPECT_TRUE(deur.isDeurOpen());
}

TEST(DeurMetSloten, SluitVergrendeltAlleSloten)
{
    TestDeur deur(0, 0, 50);
    auto slot1 = std::make_shared<domain::SleutelSlot>("A");
    auto slot2 = std::make_shared<domain::SleutelSlot>("B");
    deur.voegSlotToe(slot1);
    deur.voegSlotToe(slot2);
    slot1->ontgrendel("A");
    slot2->ontgrendel("B");
    deur.open();

    deur.sluit();

    EXPECT_TRUE(slot1->isVergrendeld());
    EXPECT_TRUE(slot2->isVergrendeld());
}

TEST(DeurMetSloten, AantalSlotenWordtGeteld)
{
    TestDeur deur(0, 0, 50);
    EXPECT_EQ(deur.aantalSloten(), 0u);

    deur.voegSlotToe(std::make_shared<domain::SleutelSlot>("A"));
    EXPECT_EQ(deur.aantalSloten(), 1u);

    deur.voegSlotToe(std::make_shared<domain::SleutelSlot>("B"));
    EXPECT_EQ(deur.aantalSloten(), 2u);
}

TEST(DeurMetSloten, AantalVergrendeldKlopt)
{
    TestDeur deur(0, 0, 50);
    auto slot1 = std::make_shared<domain::SleutelSlot>("A");
    auto slot2 = std::make_shared<domain::SleutelSlot>("B");
    auto slot3 = std::make_shared<domain::SleutelSlot>("C");
    deur.voegSlotToe(slot1);
    deur.voegSlotToe(slot2);
    deur.voegSlotToe(slot3);

    EXPECT_EQ(deur.aantalVergrendeld(), 3u);

    slot1->ontgrendel("A");
    EXPECT_EQ(deur.aantalVergrendeld(), 2u);

    slot2->ontgrendel("B");
    EXPECT_EQ(deur.aantalVergrendeld(), 1u);
}

TEST(DeurMetSloten, NullptrSlotWordtGenegeerd)
{
    TestDeur deur(0, 0, 50);
    deur.voegSlotToe(nullptr);

    EXPECT_EQ(deur.aantalSloten(), 0u)
        << "voegSlotToe(nullptr) mag de lijst niet vervuilen";
}
