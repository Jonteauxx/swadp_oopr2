/**
 * @file Draaideur.h
 * @brief Draaideur die haaks op zijn muur openzwaait (UML opdracht 1).
 * @author tj.herdigein
 * @date 2026
 */

#ifndef SWADP_OOPR2_DRAAIDEUR_H
#define SWADP_OOPR2_DRAAIDEUR_H

#include "Deur.h"

namespace domain {

/**
 * @class Draaideur
 * @brief Een Deur die scharniert in een ankerpunt.
 *
 * Dicht = in lijn met de muur; open = 90 graden gedraaid (haaks op de muur).
 *
 * Omdat dezelfde klasse zowel in een verticale als horizontale binnenmuur
 * kan zitten (d1 vs d2), neemt de constructor een Orientatie-parameter.
 *
 * @note Het `_liggend`-attribuut uit het UML is afgeleid van `_status`
 *       (open = liggend). We houden het als expliciete member om de
 *       docent-UML 1-op-1 te volgen.
 */
class Draaideur : public Deur
{
public:
    /**
     * @brief Geeft aan in welke wand-orientatie de deur scharniert.
     *
     * Belangrijk voor de teken-logica:
     *   - VerticaleWand:   dicht = lijn vertikaal, open = haaks horizontaal.
     *   - HorizontaleWand: dicht = lijn horizontaal, open = haaks verticaal.
     */
    enum class Orientatie {
        VerticaleWand,
        HorizontaleWand
    };

    /**
     * @brief Welke kant zwaait de deur op bij open.
     *  - Positief:  +X bij VerticaleWand (naar rechts),
     *               +Y bij HorizontaleWand (naar onder).
     *  - Negatief:  -X bij VerticaleWand (naar links),
     *               -Y bij HorizontaleWand (naar boven).
     */
    enum class ZwaaiRichting {
        Positief,
        Negatief
    };

    /**
     * @brief Construeer een draaideur scharnierend op (x, y).
     * @param x          Scharnier-X (pixels).
     * @param y          Scharnier-Y (pixels).
     * @param lengte     Lengte van het lijnsegment (pixels).
     * @param orientatie Oriëntatie van de wand waar de deur in zit.
     * @param richting   Welke kant zwaait de deur bij open.
     */
    Draaideur(int x, int y, unsigned lengte,
              Orientatie orientatie = Orientatie::VerticaleWand,
              ZwaaiRichting richting = ZwaaiRichting::Positief);

    /**
     * @brief Tekent het deur-segment, vorm afhankelijk van Orientatie + status.
     */
    void teken(QPaintDevice* target) override;

    void open()  override;  ///< houdt _liggend gesync met _status
    void sluit() override;  ///< houdt _liggend gesync met _status

    /// @brief true als de deur haaks staat (open).
    bool isLiggend() const;

private:
    bool          _liggend;
    Orientatie    _orientatie;
    ZwaaiRichting _zwaaiRichting;
};

} // namespace domain

#endif // SWADP_OOPR2_DRAAIDEUR_H
