/**
 * @file Drukbox.cpp
 * @brief Implementatie van de Qt-Afdrukker.
 */

#include "Drukbox.h"

#include <QString>
#include <QTextBrowser>

namespace ui {

Drukbox::Drukbox(QTextBrowser* browser)
    : _browser(browser)
{
}

void Drukbox::toonText(const std::string& tekst)
{
    if (_browser) {
        _browser->append(QString::fromStdString(tekst));
    }
}

void Drukbox::clearMedium()
{
    if (_browser) {
        _browser->clear();
    }
}

} // namespace ui
