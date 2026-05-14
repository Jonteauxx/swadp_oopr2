/**
 * @file KaartSlot.h
 * @brief Slot dat ontgrendelt met een IdKaart (opdracht 5).
 * @author tj.herdigein
 * @date 2026
 */

#ifndef SWADP_OOPR2_KAARTSLOT_H
#define SWADP_OOPR2_KAARTSLOT_H

#include "Slot.h"

#include <functional>
#include <map>
#include <string>
#include <vector>

namespace domain {

class IdKaart; // forward declaration

/**
 * @class KaartSlot
 * @brief Concreet slot dat openstaat voor IdKaarten die er toegang toe hebben.
 *
 * Volgens UML opdracht5-1.png:
 *   - `plaats`: string om dit slot fysiek te identificeren
 *   - `vergrendeld`: huidige staat
 *   - **statische** `idKaarten`: gedeelde map<string, IdKaart*> over ALLE
 *     KaartSlot-instanties (= centrale kaartenadministratie)
 *
 * Ontgrendel-flow:
 *   1. Slot krijgt een string (kaart-id) binnen.
 *   2. Het zoekt in de statische map de IdKaart op.
 *   3. Als IdKaart bekend is EN deze kaart heeft toegang tot dit slot,
 *      gaat het slot open.
 *
 * Opmerking over de statische map: deze geeft Singleton-eigenschappen
 * aan KaartSlot. Bewust gekozen omdat het UML het zo voorschrijft.
 * Trade-off: lastig te isoleren in tests (we resetten met
 * `wisAlleIdKaarten()` voorafgaand aan elke testcase).
 */
class KaartSlot : public Slot
{
public:
    /**
     * @brief Construeer een KaartSlot dat een fysieke plaats representeert.
     * @param plaats Identificatie (bv. "vd" of "d1").
     */
    explicit KaartSlot(const std::string& plaats);

    void ontgrendel(const std::string& kaartId) override;
    void vergrendel() override;
    bool isVergrendeld() const override;

    /// @return De plaats-identificatie van dit slot.
    const std::string& plaats() const;

    // --- Statische administratie -------------------------------------------

    /**
     * @brief Registreer een IdKaart in de centrale kaarten-tabel.
     *        Wordt gedeeld door ALLE KaartSlot-instanties.
     */
    static void voegIdKaartToe(IdKaart* kaart);

    /**
     * @brief Verwijder een IdKaart uit de centrale tabel.
     * @param eenId De unieke id van de te verwijderen kaart.
     */
    static void verwijderIdKaart(const std::string& eenId);

    /**
     * @brief Zoek IdKaarten in de centrale map met een predicate (lambda).
     *
     * Voorbeeld:
     * @code
     *   auto annas = KaartSlot::zoekIdKaarten(
     *       [](const IdKaart& k){ return k.naamEigenaar() == "Anna"; });
     * @endcode
     *
     * (Remedial-uitbreiding: toepassen van lambda calculus.)
     *
     * @param predicate Functie die op een IdKaart wordt uitgevoerd; geeft
     *                  true terug om de kaart in het resultaat op te nemen.
     * @return Vector met pointers naar de gevonden IdKaarten.
     */
    static std::vector<IdKaart*> zoekIdKaarten(
        const std::function<bool(const IdKaart&)>& predicate);

    /// @brief Wis alle geregistreerde IdKaarten (handig voor unit-tests).
    static void wisAlleIdKaarten();

    /// @return Aantal geregistreerde IdKaarten (handig voor tests).
    static std::size_t aantalIdKaarten();

private:
    std::string _plaats;
    bool        _vergrendeld;

    /// Statische map: gedeeld door ALLE KaartSlot-instanties.
    static std::map<std::string, IdKaart*> _idKaarten;
};

} // namespace domain

#endif // SWADP_OOPR2_KAARTSLOT_H
