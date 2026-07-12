/**
 * @file SlotException.h
 * @brief Exception die een KaartSlot gooit bij een mislukte ontgrendeling (opdracht 6).
 * @author tj.herdigein
 * @date 2026
 */

#ifndef SWADP_OOPR2_SLOTEXCEPTION_H
#define SWADP_OOPR2_SLOTEXCEPTION_H

#include <exception>
#include <string>

namespace domain {

/**
 * @class SlotException
 * @brief Wordt gegooid wanneer een IdKaart een KaartSlot niet mag/kan openen.
 *
 * Volgens UML opdracht6-1.png bevat de exception:
 *   - `plaats`: de plaats waar het KaartSlot zich bevindt;
 *   - `id`: de id van het IdKaart-object dat de exception veroorzaakte
 *           (of de tekst "geen idkaart voor xxxx" als de id onbekend is).
 *
 * Erft van std::exception zodat de exception ook via een generieke
 * `catch (const std::exception&)` opgevangen kan worden en `what()` de
 * gecombineerde melding teruggeeft.
 */
class SlotException : public std::exception
{
public:
    /**
     * @brief Construeer een exception met de plaats van het slot en de
     *        veroorzakende id.
     * @param plaats Plaats-identificatie van het KaartSlot (bv. "vd").
     * @param id     Id van de IdKaart, of "geen idkaart voor xxxx".
     */
    SlotException(const std::string& plaats, const std::string& id);

    /// @return De plaats waar het KaartSlot zich bevindt.
    const std::string& plaats() const noexcept;

    /// @return De id die de exception veroorzaakte.
    const std::string& id() const noexcept;

    /// @return Leesbare, gecombineerde melding (plaats + id).
    const char* what() const noexcept override;

private:
    std::string _plaats;
    std::string _id;
    std::string _bericht; ///< voorberekende melding voor what()
};

} // namespace domain

#endif // SWADP_OOPR2_SLOTEXCEPTION_H
