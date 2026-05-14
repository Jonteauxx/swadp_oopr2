/**
 * @file Afdrukker.h
 * @brief Interface voor het tonen van tekst (UML opdracht 4).
 * @author tj.herdigein
 * @date 2026
 */

#ifndef SWADP_OOPR2_AFDRUKKER_H
#define SWADP_OOPR2_AFDRUKKER_H

#include <string>

namespace domain {

/**
 * @class Afdrukker
 * @brief Strategy-interface voor uitvoer-media.
 *
 * Een HerkenningsSlot praat via deze interface met "iets dat tekst kan
 * weergeven" — een Drukbox (Qt-implementatie), een printer, of in tests
 * een MockAfdrukker. Daardoor is het slot niet gekoppeld aan Qt.
 *
 * Voldoet aan **Interface Segregation Principle** (alleen de twee methodes
 * die we nodig hebben - geen format/font/style ballast).
 */
class Afdrukker
{
public:
    virtual ~Afdrukker() = default;

    /**
     * @brief Toon een regel tekst op het medium.
     * @param tekst Vrij te formatteren string.
     */
    virtual void toonText(const std::string& tekst) = 0;

    /// @brief Wist het medium leeg voor een nieuwe weergave.
    virtual void clearMedium() = 0;
};

} // namespace domain

#endif // SWADP_OOPR2_AFDRUKKER_H
