/**
 * @file Draaideur.cpp
 * @brief Implementatie van Draaideur.
 */

#include "Draaideur.h"

#include <QPaintDevice>
#include <QPainter>
#include <QPen>

namespace domain {

Draaideur::Draaideur(int x, int y, unsigned lengte,
                     Orientatie orientatie, ZwaaiRichting richting)
    : Deur(x, y, lengte)
    , _liggend(false)
    , _orientatie(orientatie)
    , _zwaaiRichting(richting)
{
}

void Draaideur::open()
{
    Deur::open();
    // _liggend moet de werkelijke status volgen: als Deur::open() geweigerd
    // is (vergrendeld slot), dan blijft _liggend false en tekent de deur
    // nog steeds als dicht. Bug-fix t.o.v. eerder altijd-true zetten.
    _liggend = _status;
}

void Draaideur::sluit()
{
    Deur::sluit();
    _liggend = _status;
}

bool Draaideur::isLiggend() const
{
    return _liggend;
}

void Draaideur::teken(QPaintDevice* target)
{
    QPainter painter(target);

    // Kleur op basis van slot-staat: rood = vergrendeld, zwart = ontgrendeld.
    const QColor kleur = (_mijnSlot && _mijnSlot->isVergrendeld())
                       ? QColor(220, 50, 50)
                       : Qt::black;
    QPen pen(kleur, 5, Qt::SolidLine, Qt::FlatCap);
    painter.setPen(pen);

    const int L     = static_cast<int>(_lengte);
    const int delta = (_zwaaiRichting == ZwaaiRichting::Positief) ? L : -L;

    if (_orientatie == Orientatie::VerticaleWand) {
        // Dicht  = verticaal in muur (altijd naar onder vanaf scharnier-top).
        // Open   = horizontaal haaks, richting volgens _zwaaiRichting.
        if (_liggend) {
            painter.drawLine(_x_coordinaat,         _y_coordinaat,
                             _x_coordinaat + delta, _y_coordinaat);
        } else {
            painter.drawLine(_x_coordinaat, _y_coordinaat,
                             _x_coordinaat, _y_coordinaat + L);
        }
    } else {
        // Dicht  = horizontaal in muur (altijd naar rechts vanaf scharnier-links).
        // Open   = verticaal haaks, richting volgens _zwaaiRichting.
        if (_liggend) {
            painter.drawLine(_x_coordinaat, _y_coordinaat,
                             _x_coordinaat, _y_coordinaat + delta);
        } else {
            painter.drawLine(_x_coordinaat,     _y_coordinaat,
                             _x_coordinaat + L, _y_coordinaat);
        }
    }
}

} // namespace domain
