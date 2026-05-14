/**
 * @file Deur.h
 * @brief Abstracte basis-klasse voor deuren (UML opdracht 1-3).
 * @author tj.herdigein
 * @date 2026
 */

#ifndef SWADP_OOPR2_DEUR_H
#define SWADP_OOPR2_DEUR_H

#include "Slot.h"

#include <memory>
#include <vector>

class QPaintDevice;

namespace domain {

/**
 * @class Deur
 * @brief Abstracte basis voor alle deur-types met 0..n sloten (opdracht 3).
 *
 * Volgens UML opdracht3_1.png:
 *   - Een Deur heeft een collectie sloten (`vector<shared_ptr<Slot>>`).
 *   - `open()` werkt alleen als ALLE sloten ontgrendeld zijn.
 *   - `sluit()` vergrendelt ALLE sloten (per opdracht 2 voor één slot,
 *     uitgebreid naar alle sloten voor opdracht 3).
 *
 * De keuze voor `shared_ptr` (i.p.v. raw pointer of unique_ptr) komt omdat
 * MainWindow en Deur beide het slot kunnen aanspreken (MainWindow voor
 * (ont)grendelen via UI, Deur voor de open/sluit-check).
 */
class Deur
{
public:
    Deur(int x, int y, unsigned lengte);
    virtual ~Deur() = default;

    /**
     * @brief Opent de deur als alle sloten ontgrendeld zijn.
     *
     * Bij een vergrendeld slot blijft de deur dicht (status onveranderd).
     */
    virtual void open();

    /**
     * @brief Sluit de deur en vergrendelt automatisch ALLE bijbehorende
     *        sloten (gedrag uit opdracht 2, uitgebreid voor opdracht 3).
     */
    virtual void sluit();

    virtual void teken(QPaintDevice* target) = 0;

    bool     isDeurOpen() const;
    unsigned deurLengte() const;

    /**
     * @brief Voegt een slot toe aan deze deur.
     * @param slot shared_ptr naar het toe te voegen slot. Een nullptr wordt
     *             geaccepteerd maar genegeerd.
     */
    void voegSlotToe(std::shared_ptr<Slot> slot);

    /**
     * @brief @return true als alle sloten ontgrendeld zijn (of als de deur
     *         géén sloten heeft).
     */
    bool alleSlotenOntgrendeld() const;

    /**
     * @brief @return aantal sloten dat momenteel nog vergrendeld is.
     *         Handig voor UI-statuslabels ("1/2 vergrendeld").
     */
    std::size_t aantalVergrendeld() const;

    /**
     * @brief @return totaal aantal sloten op deze deur.
     */
    std::size_t aantalSloten() const;

protected:
    bool     _status;
    int      _x_coordinaat;
    int      _y_coordinaat;
    unsigned _lengte;
    std::vector<std::shared_ptr<Slot>> _sloten;
};

} // namespace domain

#endif // SWADP_OOPR2_DEUR_H
