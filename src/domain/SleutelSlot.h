/**
 * @file SleutelSlot.h
 * @brief Concreet slot dat alleen met een geheime string ontgrendelt.
 * @author tj.herdigein
 * @date 2026
 */

#ifndef SWADP_OOPR2_SLEUTELSLOT_H
#define SWADP_OOPR2_SLEUTELSLOT_H

#include "Slot.h"

#include <string>

namespace domain {

/**
 * @class SleutelSlot
 * @brief Slot dat ontgrendelt zodra de aangeboden sleutel gelijk is aan
 *        de in de constructor opgegeven sleutel.
 *
 * Bij constructie is het slot vergrendeld.
 */
class SleutelSlot : public Slot
{
public:
    /**
     * @brief Construeer een SleutelSlot met de geheime sleutel.
     * @param sleutel De string die het slot kan ontgrendelen.
     */
    explicit SleutelSlot(const std::string& sleutel);

    void ontgrendel(const std::string& eenSleutel) override;
    void vergrendel() override;
    bool isVergrendeld() const override;

private:
    std::string _sleutel;     ///< de juiste sleutel
    bool        _vergrendeld; ///< huidige staat
};

} // namespace domain

#endif // SWADP_OOPR2_SLEUTELSLOT_H
