/**
 * @file Schuifdeur.h
 * @brief Schuifdeur met optionele controle-sensor (UML opdracht 1).
 * @author tj.herdigein
 * @date 2026
 */

#ifndef SWADP_OOPR2_SCHUIFDEUR_H
#define SWADP_OOPR2_SCHUIFDEUR_H

#include "Deur.h"

namespace domain {

class Sensor;

/**
 * @class Schuifdeur
 * @brief Een Deur die zijwaarts schuift en optioneel een hal-sensor controleert.
 *
 * Toegevoegd t.o.v. Deur:
 *   - Aggregatie met een `Sensor*` (de hall-sensor naast de deur).
 *     Schuifdeur is *niet* eigenaar van de sensor.
 *   - Override `teken`: lijnsegment over volle lengte als dicht,
 *     halve lengte als open (de "weggeschoven" helft).
 *   - Override `sluit`: weigert te sluiten zolang de sensor actief is
 *     (alsof een magneet de hall-sensor triggert).
 */
class Schuifdeur : public Deur
{
public:
    /**
     * @brief Construeer een schuifdeur op (x, y) met gegeven lengte.
     * @param x       X-coordinaat van het ankerpunt (bovenkant).
     * @param y       Y-coordinaat van het ankerpunt.
     * @param lengte  Lengte van het lijnsegment (pixels).
     * @param sensor  Optionele aggregatie met een hal-sensor;
     *                Schuifdeur wordt *geen* eigenaar.
     */
    Schuifdeur(int x, int y, unsigned lengte, Sensor* sensor = nullptr);

    /**
     * @brief Probeer de deur te sluiten.
     *
     * Werkt alleen als er geen sensor is, of als de sensor niet
     * geactiveerd is. Anders blijft de deur open ("magneet houdt
     * hem tegen").
     */
    void sluit() override;

private:
    Sensor* _mijnSensor;  ///< Aggregatie: niet-eigenaar; nullptr toegestaan.
};

} // namespace domain

#endif // SWADP_OOPR2_SCHUIFDEUR_H
