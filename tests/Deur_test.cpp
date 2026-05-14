/**
 * @file Deur_test.cpp
 * @brief Unit-tests voor domain::Deur (basis-gedrag + slot-interactie).
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

// -----------------------------------------------------------------------------
// Slot-interactie (UML opdracht 2)
// -----------------------------------------------------------------------------

TEST(DeurMetSlot, ZonderSlotOpenBlijftWerken)
{
    TestDeur deur(0, 0, 50);
    deur.open();
    EXPECT_TRUE(deur.isDeurOpen());
}

TEST(DeurMetSlot, OpenWeigertBijVergrendeldSlot)
{
    TestDeur deur(0, 0, 50);
    deur.setSlot(std::make_shared<domain::SleutelSlot>("geheim"));
    // Slot start vergrendeld.

    deur.open();

    EXPECT_FALSE(deur.isDeurOpen())
        << "Een deur met vergrendeld slot mag niet open gaan";
}

TEST(DeurMetSlot, OpenWerktNaSlotOntgrendelen)
{
    TestDeur deur(0, 0, 50);
    auto slot = std::make_shared<domain::SleutelSlot>("geheim");
    deur.setSlot(slot);

    slot->ontgrendel("geheim");
    deur.open();

    EXPECT_TRUE(deur.isDeurOpen());
}

TEST(DeurMetSlot, NaSlotOpnieuwVergrendelenGaatDeurNietMeerOpen)
{
    TestDeur deur(0, 0, 50);
    auto slot = std::make_shared<domain::SleutelSlot>("geheim");
    deur.setSlot(slot);
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
    deur.setSlot(slot);
    slot->ontgrendel("geheim");
    deur.open();
    ASSERT_TRUE(deur.isDeurOpen());

    deur.sluit();

    EXPECT_TRUE(slot->isVergrendeld())
        << "Per opdracht 2: bij sluit() moet het slot automatisch vergrendeld worden";
}
