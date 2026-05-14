/**
 * @file Servo.h
 * @brief Hoog-niveau wrapper rond een servomotor via IGpio.
 * @author tj.herdigein
 * @date 2026
 */

#ifndef SWADP_OOPR2_SERVO_H
#define SWADP_OOPR2_SERVO_H

#include "IGpio.h"

namespace infra {

/**
 * @class Servo
 * @brief Vertaalt logische "open" / "dicht" commando's naar pulsewidths via IGpio.
 *
 * De Servo bezit GEEN IGpio (referentie-aggregatie). De eigenaar van de
 * IGpio (typisch MainWindow of GebouwController) moet langer leven dan
 * de Servo's die hem gebruiken.
 *
 * Bij constructie wordt de servo direct naar de dichte stand gebracht
 * zodat de fysieke positie matched met de logische beginstaat.
 */
class Servo
{
public:
    /**
     * @brief Construeer een servo op een BCM-pin met twee gedefinieerde hoeken.
     *
     * @param gpio       Hardware-laag (IGpio, mockable).
     * @param bcmPin     BCM-pin van het PWM-signaal (bv. 18 voor schuifdeur).
     * @param hoekDicht  Hoek in graden voor "dicht" stand (0..180).
     * @param hoekOpen   Hoek in graden voor "open" stand (0..180).
     */
    Servo(IGpio& gpio, int bcmPin, int hoekDicht, int hoekOpen);

    /// @brief Beweeg de servo naar de open-hoek.
    void zetOpen();

    /// @brief Beweeg de servo naar de dicht-hoek.
    void zetDicht();

    /// @brief Geef de hoek terug die als laatste gestuurd is.
    int huidigeHoek() const;

private:
    IGpio& _gpio;
    int    _bcmPin;
    int    _hoekDicht;
    int    _hoekOpen;
    int    _huidigeHoek;
};

} // namespace infra

#endif // SWADP_OOPR2_SERVO_H
