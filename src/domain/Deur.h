/**
 * @file Deur.h
 * @brief Abstracte basis-klasse voor deuren in het gebouw (UML opdracht 1).
 * @author tj.herdigein
 * @date 2026
 */

#ifndef SWADP_OOPR2_DEUR_H
#define SWADP_OOPR2_DEUR_H

class QPaintDevice;

namespace domain {

/**
 * @class Deur
 * @brief Abstracte basis voor alle deur-types (Schuifdeur, Draaideur, ...).
 *
 * Volgens het docent-diagram (opdracht1_4.png) heeft elke deur:
 *   - een open/dicht-toestand (`status`),
 *   - een (x, y)-positie in het venster,
 *   - een lengte (voor het lijnsegment dat de deur weergeeft).
 *
 * De manier van tekenen verschilt per type en is daarom pure virtual.
 * Sloten en sensoren worden in latere opdrachten toegevoegd.
 *
 * @note Polymorfisme: een vector<Deur*> kan zowel Schuifdeur als
 *       Draaideur bevatten - daar maakt MainWindow gebruik van.
 */
class Deur
{
public:
    /**
     * @brief Construeer een deur op een gegeven positie en lengte.
     * @param x      X-coordinaat (pixels).
     * @param y      Y-coordinaat (pixels).
     * @param lengte Lengte van het lijnsegment dat de deur tekent (pixels).
     */
    Deur(int x, int y, unsigned lengte);

    virtual ~Deur() = default;

    /// @brief Zet de deur op open (status = true).
    virtual void open();

    /// @brief Zet de deur op dicht (status = false).
    /// Subklassen kunnen dit overriden met extra voorwaarden
    /// (zie Schuifdeur::sluit).
    virtual void sluit();

    /**
     * @brief Teken de deur op het opgegeven QPaintDevice.
     *
     * Pure virtual; subklassen bepalen of het lijnsegment in lijn met
     * de muur of haaks erop wordt getekend.
     *
     * @param target Qt paint-device (bv. een MainWindow).
     */
    virtual void teken(QPaintDevice* target) = 0;

    /// @brief Lees de huidige open/dicht-toestand.
    /// @return true als de deur open is.
    bool isDeurOpen() const;

    /// @brief Geef de lengte (in pixels) van het deur-lijnsegment terug.
    unsigned deurLengte() const;

protected:
    bool     _status;        ///< false = dicht, true = open
    int      _x_coordinaat;
    int      _y_coordinaat;
    unsigned _lengte;
};

} // namespace domain

#endif // SWADP_OOPR2_DEUR_H
