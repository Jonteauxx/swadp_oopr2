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
 * @brief Een Deur die scharniert in een ankerpunt: dicht = in lijn met
 *        de muur, open = 90 graden gedraaid (haaks op de muur).
 *
 * In opdracht 1 zit deze in een verticale binnenmuur:
 *   - Dicht: lijn verticaal omlaag vanaf (x, y).
 *   - Open : lijn horizontaal naar rechts vanaf (x, y).
 *
 * Het `_liggend` veld is een UML-attribuut uit het docent-diagram en
 * wordt automatisch synchroon gehouden met de open/dicht-status.
 */
class Draaideur : public Deur
{
public:
    /**
     * @brief Construeer een draaideur scharnierend op (x, y).
     * @param x       Scharnier-X (pixels).
     * @param y       Scharnier-Y (pixels).
     * @param lengte  Lengte van het lijnsegment (pixels).
     */
    Draaideur(int x, int y, unsigned lengte);

    /**
     * @brief Tekent het deursegment.
     *  - Dicht: verticaal omlaag (in lijn met de muur).
     *  - Open : horizontaal naar rechts (haaks op de muur).
     * @param target Qt paint-device.
     */
    void teken(QPaintDevice* target) override;

    void open()  override;  ///< houdt _liggend gesync met _status
    void sluit() override;  ///< houdt _liggend gesync met _status

    /// @brief true als de deur haaks staat (open).
    bool isLiggend() const;

private:
    bool _liggend;  ///< true wanneer open (haaks); false wanneer dicht.
};

} // namespace domain

#endif // SWADP_OOPR2_DRAAIDEUR_H
