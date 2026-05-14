/**
 * @file IdKaart_test.cpp
 * @brief Unit-tests voor IdKaart - basis-API en operator+.
 */

#include "domain/IdKaart.h"
#include "domain/KaartSlot.h"

#include <gtest/gtest.h>

using domain::IdKaart;
using domain::KaartSlot;

class IdKaartTest : public ::testing::Test
{
protected:
    void SetUp() override { KaartSlot::wisAlleIdKaarten(); }
};

TEST_F(IdKaartTest, ConstructorBewaardIdNaamAdres)
{
    IdKaart k("K001", "Anna", "Hoofdstraat 1");

    EXPECT_EQ(k.userId(), "K001");
    EXPECT_EQ(k.naamEigenaar(), "Anna");
    EXPECT_EQ(k.adresEigenaar(), "Hoofdstraat 1");
}

TEST_F(IdKaartTest, ToegangBeginLeeg)
{
    IdKaart k("K001", "Anna", "Adres");
    EXPECT_TRUE(k.toegang().empty());
}

TEST_F(IdKaartTest, GeefToegangVoegtSlotToe)
{
    IdKaart k("K001", "Anna", "Adres");
    KaartSlot slot("vd");

    k.geefToegang(&slot);

    ASSERT_EQ(k.toegang().size(), 1u);
    EXPECT_EQ(k.toegang()[0], &slot);
}

TEST_F(IdKaartTest, HeeftToegangTotKlopt)
{
    IdKaart k("K001", "Anna", "Adres");
    KaartSlot slotA("vd");
    KaartSlot slotB("d1");

    k.geefToegang(&slotA);

    EXPECT_TRUE(k.heeftToegangTot(&slotA));
    EXPECT_FALSE(k.heeftToegangTot(&slotB));
}

TEST_F(IdKaartTest, VerwijderToegangHaaltSlotEruit)
{
    IdKaart k("K001", "Anna", "Adres");
    KaartSlot slot("vd");
    k.geefToegang(&slot);

    k.verwijderToegang(&slot);

    EXPECT_FALSE(k.heeftToegangTot(&slot));
    EXPECT_TRUE(k.toegang().empty());
}

TEST_F(IdKaartTest, GeefToegangIsIdempotent)
{
    IdKaart k("K001", "Anna", "Adres");
    KaartSlot slot("vd");

    k.geefToegang(&slot);
    k.geefToegang(&slot);
    k.geefToegang(&slot);

    EXPECT_EQ(k.toegang().size(), 1u)
        << "Hetzelfde slot tweemaal toevoegen moet niet leiden tot duplicaten";
}

// =============================================================================
// operator+ : vereniging van toegekende KaartSloten (remedial uitbreiding)
// =============================================================================

TEST_F(IdKaartTest, OperatorPlusGeefVerenigingVanToegang)
{
    IdKaart a("KA", "Anna",  "A1");
    IdKaart b("KB", "Bob",   "B1");

    KaartSlot s1("vd");
    KaartSlot s2("d1");
    KaartSlot s3("d2");

    a.geefToegang(&s1);
    a.geefToegang(&s2);
    b.geefToegang(&s2);
    b.geefToegang(&s3);

    auto vereniging = a + b;

    EXPECT_EQ(vereniging.size(), 3u);
    EXPECT_EQ(vereniging.count(&s1), 1u);
    EXPECT_EQ(vereniging.count(&s2), 1u);
    EXPECT_EQ(vereniging.count(&s3), 1u);
}

TEST_F(IdKaartTest, OperatorPlusMetLegeKaartOK)
{
    IdKaart a("KA", "Anna", "A1");
    IdKaart b("KB", "Bob",  "B1");
    KaartSlot s1("vd");
    a.geefToegang(&s1);

    auto vereniging = a + b;

    EXPECT_EQ(vereniging.size(), 1u);
    EXPECT_EQ(vereniging.count(&s1), 1u);
}
