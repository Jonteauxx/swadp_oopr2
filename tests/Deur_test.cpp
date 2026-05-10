/**
 * @file Deur_test.cpp
 * @brief Unit-tests voor domain::Deur (basis-gedrag, zonder tekenen).
 */

#include "domain/Deur.h"

#include <gtest/gtest.h>

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
