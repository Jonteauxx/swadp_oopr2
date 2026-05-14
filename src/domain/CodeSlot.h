/**
 * @file CodeSlot.h
 * @brief Concreet slot dat met een numerieke code ontgrendelt.
 * @author tj.herdigein
 * @date 2026
 */

#ifndef SWADP_OOPR2_CODESLOT_H
#define SWADP_OOPR2_CODESLOT_H

#include "Slot.h"

#include <string>

namespace domain {

/**
 * @class CodeSlot
 * @brief Slot dat ontgrendelt zodra de aangeboden string overeenkomt met
 *        de in de constructor opgegeven numerieke code.
 *
 * De code wordt intern als int bewaard (zoals het UML van opdracht2_1
 * voorschrijft: `code: Integer`), maar ontgrendel ontvangt een string
 * omdat dat is wat een toetsenbord-input doorgaans levert. De string
 * wordt intern naar int geparsed en vergeleken.
 */
class CodeSlot : public Slot
{
public:
    /**
     * @brief Construeer een CodeSlot met de geheime code.
     * @param code De integer-code die het slot kan ontgrendelen.
     */
    explicit CodeSlot(int code);

    void ontgrendel(const std::string& eenCode) override;
    void vergrendel() override;
    bool isVergrendeld() const override;

private:
    int  _code;        ///< de juiste code
    bool _vergrendeld; ///< huidige staat
};

} // namespace domain

#endif // SWADP_OOPR2_CODESLOT_H
