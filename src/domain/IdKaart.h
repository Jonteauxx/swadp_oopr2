/**
 * @file IdKaart.h
 * @brief Toegangskaart met persoonsgegevens en lijst van toegestane KaartSloten.
 * @author tj.herdigein
 * @date 2026
 */

#ifndef SWADP_OOPR2_IDKAART_H
#define SWADP_OOPR2_IDKAART_H

#include <set>
#include <string>
#include <vector>

namespace domain {

// Forward declaration: KaartSlot en IdKaart verwijzen wederzijds naar elkaar.
class KaartSlot;

/**
 * @class IdKaart
 * @brief Een toegangskaart met unieke id, naam + adres van de eigenaar,
 *        en een lijst van KaartSlot-objecten waar deze kaart toegang toe geeft.
 *
 * Volgens UML opdracht5-1.png:
 *   -id : string
 *   -toegang : vector<KaartSlot*>
 *   -naamEigenaar : string
 *   -adresEigenaar : string
 *   +geefToegang(KaartSlot*)
 *   +verwijderToegang(KaartSlot*)
 *   +heeftToegangTot(KaartSlot*) : bool
 *
 * Plus (remedial-uitbreiding):
 *   - operator+ : combineert toegang van twee kaarten (set-vereniging).
 */
class IdKaart
{
public:
    /**
     * @brief Construeer een IdKaart met unieke id en persoonsgegevens.
     * @param id     Uniek nummer/string voor deze kaart.
     * @param naam   Naam van de eigenaar.
     * @param adres  Adres van de eigenaar.
     */
    IdKaart(const std::string& id,
            const std::string& naam,
            const std::string& adres);

    /// @return Unieke id van de kaart (key in KaartSlot::idKaarten).
    const std::string& userId()       const;
    const std::string& naamEigenaar() const;
    const std::string& adresEigenaar() const;

    /// @brief Voeg een KaartSlot toe waar deze kaart toegang toe heeft.
    void geefToegang(KaartSlot* slot);

    /// @brief Verwijder een KaartSlot uit de toegang-lijst.
    void verwijderToegang(KaartSlot* slot);

    /// @return true als deze kaart toegang heeft tot het opgegeven slot.
    bool heeftToegangTot(KaartSlot* slot) const;

    /// @return Read-only view op de toegang-vector (voor operator+ en tests).
    const std::vector<KaartSlot*>& toegang() const;

private:
    std::string             _id;
    std::string             _naamEigenaar;
    std::string             _adresEigenaar;
    std::vector<KaartSlot*> _toegang;
};

/**
 * @brief Operator+: vereniging van de toegang van twee IdKaarten (remedial).
 *
 * Voorbeeld: als kaart A toegang heeft tot {S1, S2} en kaart B tot {S2, S3},
 * dan is A + B == {S1, S2, S3}. std::set garandeert geen duplicaten.
 *
 * @return Set met alle unieke KaartSloten waartoe a OF b toegang heeft.
 */
std::set<KaartSlot*> operator+(const IdKaart& a, const IdKaart& b);

} // namespace domain

#endif // SWADP_OOPR2_IDKAART_H
