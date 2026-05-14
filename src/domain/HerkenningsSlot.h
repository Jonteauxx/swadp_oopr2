/**
 * @file HerkenningsSlot.h
 * @brief Slot dat ontgrendelt op basis van een naam-autorisatie-tabel (opdracht 4).
 * @author tj.herdigein
 * @date 2026
 */

#ifndef SWADP_OOPR2_HERKENNINGSSLOT_H
#define SWADP_OOPR2_HERKENNINGSSLOT_H

#include "Afdrukker.h"
#include "Slot.h"

#include <map>
#include <string>

namespace domain {

/**
 * @class HerkenningsSlot
 * @brief Slot met een interne "kaartenbak" - lijst van namen en of die
 *        wel of niet binnen mogen.
 *
 * Bij ontgrendel(naam) wordt opgezocht of de naam:
 *   - bekend is in de kaartenbak EN value == true  -> slot opent;
 *   - bekend is met value == false                  -> slot blijft dicht
 *     (deze persoon mag er expliciet niet in);
 *   - onbekend is                                    -> slot blijft dicht.
 *
 * De inhoud van de kaartenbak kan zichtbaar gemaakt worden via een
 * `Afdrukker` (typisch een Drukbox die een QTextBrowser vult).
 */
class HerkenningsSlot : public Slot
{
public:
    /**
     * @brief Construeer een HerkenningsSlot.
     * @param afdrukker Optionele afdrukker (niet-eigenaar pointer);
     *                  nullptr betekent: kaartenbak kan niet worden afgedrukt.
     */
    explicit HerkenningsSlot(Afdrukker* afdrukker = nullptr);

    void ontgrendel(const std::string& naam) override;
    void vergrendel() override;
    bool isVergrendeld() const override;

    /**
     * @brief Voeg een naam toe (of update zijn autorisatie).
     * @param naam            Naam van de persoon.
     * @param magNaarBinnen   true = mag de deur openen, false = geblokkeerd.
     */
    void voegAutorissatieToe(const std::string& naam, bool magNaarBinnen);

    /// @brief Toon de complete kaartenbak via de gekoppelde afdrukker.
    void toonKaartenbak();

private:
    std::map<std::string, bool> _kaartenbak;
    bool        _vergrendeld;
    Afdrukker*  _mijnAfdrukker;
};

} // namespace domain

#endif // SWADP_OOPR2_HERKENNINGSSLOT_H
