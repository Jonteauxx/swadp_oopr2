/**
 * @file HerkenningsSlot_test.cpp
 * @brief Unit-tests voor domain::HerkenningsSlot.
 *
 * Gebruikt een lokale MockAfdrukker om de Afdrukker-interaktie te
 * verifieren zonder Qt-afhankelijkheid.
 */

#include "domain/Afdrukker.h"
#include "domain/HerkenningsSlot.h"

#include <gtest/gtest.h>

#include <vector>

namespace {

/// Mock-afdrukker: bewaart alle aangeroepen acties zodat tests kunnen
/// controleren WAT er geprint zou worden.
class MockAfdrukker : public domain::Afdrukker
{
public:
    void toonText(const std::string& tekst) override
    {
        regels.push_back(tekst);
    }
    void clearMedium() override
    {
        regels.clear();
        ++clearCount;
    }

    std::vector<std::string> regels;
    int clearCount = 0;
};

} // anonymous namespace

using domain::HerkenningsSlot;

// =============================================================================
// Basis gedrag - opent/sluit op basis van kaartenbak
// =============================================================================

TEST(HerkenningsSlot, StartVergrendeld)
{
    HerkenningsSlot slot;
    EXPECT_TRUE(slot.isVergrendeld());
}

TEST(HerkenningsSlot, OntgrendelMetGeauthoriseerdeNaamOpent)
{
    HerkenningsSlot slot;
    slot.voegAutorissatieToe("Anna", true);

    slot.ontgrendel("Anna");

    EXPECT_FALSE(slot.isVergrendeld());
}

TEST(HerkenningsSlot, OntgrendelMetOnbekendeNaamBlijftVergrendeld)
{
    HerkenningsSlot slot;
    slot.voegAutorissatieToe("Anna", true);

    slot.ontgrendel("OnbekendePersoon");

    EXPECT_TRUE(slot.isVergrendeld());
}

TEST(HerkenningsSlot, OntgrendelMetGeblokkeerdeNaamBlijftVergrendeld)
{
    HerkenningsSlot slot;
    slot.voegAutorissatieToe("Bert", false); // er staat: Bert mag NIET

    slot.ontgrendel("Bert");

    EXPECT_TRUE(slot.isVergrendeld())
        << "Bert staat in de kaartenbak maar mag NIET binnen";
}

TEST(HerkenningsSlot, VergrendelZetTerug)
{
    HerkenningsSlot slot;
    slot.voegAutorissatieToe("Anna", true);
    slot.ontgrendel("Anna");
    ASSERT_FALSE(slot.isVergrendeld());

    slot.vergrendel();

    EXPECT_TRUE(slot.isVergrendeld());
}

TEST(HerkenningsSlot, AutorisatieKanWordenBijgewerkt)
{
    HerkenningsSlot slot;
    slot.voegAutorissatieToe("Bert", false); // eerst geblokkeerd
    slot.voegAutorissatieToe("Bert", true);  // dan vrijgegeven

    slot.ontgrendel("Bert");

    EXPECT_FALSE(slot.isVergrendeld());
}

// =============================================================================
// Afdrukker-interactie - toonKaartenbak
// =============================================================================

TEST(HerkenningsSlot, ToonKaartenbakRoeptAfdrukkerAan)
{
    MockAfdrukker afdr;
    HerkenningsSlot slot(&afdr);
    slot.voegAutorissatieToe("Anna", true);
    slot.voegAutorissatieToe("Bert", false);

    slot.toonKaartenbak();

    EXPECT_GE(afdr.regels.size(), 2u)
        << "Verwacht minstens een regel per kaartenbak-entry";
}

TEST(HerkenningsSlot, ToonKaartenbakCleartEersteHetMedium)
{
    MockAfdrukker afdr;
    HerkenningsSlot slot(&afdr);
    slot.voegAutorissatieToe("Anna", true);

    slot.toonKaartenbak();

    EXPECT_EQ(afdr.clearCount, 1)
        << "Voor het tonen hoort het medium leeggemaakt te worden";
}

TEST(HerkenningsSlot, ToonKaartenbakZonderAfdrukkerCrashtNiet)
{
    HerkenningsSlot slot(nullptr);
    slot.voegAutorissatieToe("Anna", true);

    // Mag GEEN crash veroorzaken; gewoon stil falen.
    EXPECT_NO_THROW(slot.toonKaartenbak());
}
