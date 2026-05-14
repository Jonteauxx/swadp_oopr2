/**
 * @file Slot.h
 * @brief Abstracte interface voor sloten op deuren (UML opdracht 2).
 * @author tj.herdigein
 * @date 2026
 */

#ifndef SWADP_OOPR2_SLOT_H
#define SWADP_OOPR2_SLOT_H

#include <string>

namespace domain {

/**
 * @class Slot
 * @brief Strategy-interface voor verschillende slot-types (Sleutel, Code,
 *        Herkenning, Kaart).
 *
 * Een Deur weet niets van het concrete slot-type — alleen van deze
 * interface. Daardoor:
 *   - Voldoen we aan Open/Closed (nieuw slot-type = subklasse erbij,
 *     Deur blijft ongemoeid).
 *   - Voldoen we aan Liskov (elke slot kan een Slot vervangen).
 *   - Implementeren we het GoF **Strategy** pattern in praktijk.
 *
 * Volgens UML opdracht2_1.png:
 *   +ontgrendel(eenSleutel: String): void
 *   +vergrendel(): void
 *   +isVergrendeld(): boolean
 */
class Slot
{
public:
    virtual ~Slot() = default;

    /**
     * @brief Probeer het slot te ontgrendelen met een sleutel/code.
     *
     * Of de poging slaagt is aan de concrete subklasse — sommigen
     * controleren stringvergelijking, andere doen string-to-int parsing.
     *
     * @param eenSleutel De ingevoerde sleutel (kan een string van een
     *                   sleutel of een code zijn).
     */
    virtual void ontgrendel(const std::string& eenSleutel) = 0;

    /// @brief Vergrendel het slot weer.
    virtual void vergrendel() = 0;

    /// @brief Lees de huidige staat.
    /// @return true als het slot vergrendeld is.
    virtual bool isVergrendeld() const = 0;
};

} // namespace domain

#endif // SWADP_OOPR2_SLOT_H
