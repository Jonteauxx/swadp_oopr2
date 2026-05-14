/**
 * @file Drukbox.h
 * @brief Qt-implementatie van Afdrukker - gebruikt een QTextBrowser als medium.
 * @author tj.herdigein
 * @date 2026
 */

#ifndef SWADP_OOPR2_DRUKBOX_H
#define SWADP_OOPR2_DRUKBOX_H

#include "domain/Afdrukker.h"

class QTextBrowser;

namespace ui {

/**
 * @class Drukbox
 * @brief Concrete Afdrukker die regels uitschrijft op een Qt QTextBrowser.
 *
 * Architectuur-noot: Drukbox zit in de UI-laag (gebruikt Qt), maar
 * implementeert de domein-interface `Afdrukker`. Daardoor blijft de
 * domeinlaag (HerkenningsSlot) onafhankelijk van Qt en testbaar met
 * een mock.
 *
 * Drukbox houdt een niet-eigenaar pointer naar de QTextBrowser. De
 * QTextBrowser leeft als Qt-child van MainWindow en moet langer leven
 * dan deze Drukbox.
 */
class Drukbox : public domain::Afdrukker
{
public:
    /// @param browser Qt-widget waar tekst op getoond wordt. Mag niet null.
    explicit Drukbox(QTextBrowser* browser);

    void toonText(const std::string& tekst) override;
    void clearMedium() override;

private:
    QTextBrowser* _browser;
};

} // namespace ui

#endif // SWADP_OOPR2_DRUKBOX_H
