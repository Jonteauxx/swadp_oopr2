/**
 * @file MainWindow.cpp
 * @brief Implementatie van het hoofdvenster.
 */

#include "MainWindow.h"

#include <QDebug>
#include <QFont>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>

namespace ui {

// -----------------------------------------------------------------------------
// Constanten - posities in venster-coordinaten.
// PNG zit op (20, 20) en is 500x400, dus PNG-rechterrand zit op x=520,
// PNG-onderrand op y=420. Coordinaten hieronder zijn empirisch en kunnen
// tijdens visueel testen op de Pi worden bijgeschaafd.
// -----------------------------------------------------------------------------

namespace {
    constexpr int VENSTER_BREEDTE  = 900;
    constexpr int VENSTER_HOOGTE   = 600;
    constexpr int PNG_OFFSET_X     = 20;
    constexpr int PNG_OFFSET_Y     = 20;

    // Schuifdeur (vd) - rechterwand van het gebouw
    // Anker = bovenkant van het wand-gat; lijnsegment loopt naar beneden.
    constexpr int VD_X       = 529;
    constexpr int VD_Y       = 213;
    constexpr int VD_LENGTE  = 50;

    // Draaideur d1 - verticale wand rechts van kamer 1 (linksboven)
    // Scharnier aan de bovenkant van het wand-gat, draait open naar rechts.
    constexpr int D1_X       = 252;
    constexpr int D1_Y       = 107;
    constexpr int D1_LENGTE  = 35;

    // Draaideur d2 - horizontale wand boven het kamertje rechtsbeneden
    // Scharnier aan de linkerkant van het wand-gat, draait open naar onder.
    constexpr int D2_X       = 301;
    constexpr int D2_Y       = 318;
    constexpr int D2_LENGTE  = 35;

    // HallSensor (s1) - rechts naast de schuifdeur, buiten het gebouw.
    // QPainter::drawEllipse(x, y, w, h) tekent vanaf linkerboven, dus
    // we trekken de helft van de diameter af om de cirkel rond de klik te krijgen.
    constexpr int S1_X       = 524;
    constexpr int S1_Y       = 208;

    // Knoppen-kolom rechts naast het gebouw
    constexpr int BTN_X      = 560;
    constexpr int BTN_BREED  = 150;
    constexpr int BTN_HOOG   = 32;
}

MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent)
    , _gebouw(":/assets/Gebouw.png")
    , _halsensor(S1_X, S1_Y)
    , _vd(VD_X, VD_Y, VD_LENGTE, &_halsensor)
    , _d1(D1_X, D1_Y, D1_LENGTE, domain::Draaideur::Orientatie::VerticaleWand)
    , _d2(D2_X, D2_Y, D2_LENGTE, domain::Draaideur::Orientatie::HorizontaleWand)
{
    setFixedSize(VENSTER_BREEDTE, VENSTER_HOOGTE);
    setWindowTitle("L&B GebouwBeheer");

    // --- Knoppen ------------------------------------------------------------
    auto* btnVd = new QPushButton("vd (schuifdeur)", this);
    btnVd->setGeometry(BTN_X, 220, BTN_BREED, BTN_HOOG);
    connect(btnVd, &QPushButton::clicked,
            this, &MainWindow::onSchuifdeurKnopClicked);

    auto* btnD1 = new QPushButton("d1 (draaideur)", this);
    btnD1->setGeometry(BTN_X, 95, BTN_BREED, BTN_HOOG);
    connect(btnD1, &QPushButton::clicked,
            this, &MainWindow::onDraaideurD1KnopClicked);

    auto* btnD2 = new QPushButton("d2 (draaideur)", this);
    btnD2->setGeometry(BTN_X, 270, BTN_BREED, BTN_HOOG);
    connect(btnD2, &QPushButton::clicked,
            this, &MainWindow::onDraaideurD2KnopClicked);

    auto* btnSens = new QPushButton("halsensor toggle", this);
    btnSens->setGeometry(BTN_X, 340, BTN_BREED, BTN_HOOG);
    connect(btnSens, &QPushButton::clicked,
            this, &MainWindow::onHalsensorKnopClicked);
}

void MainWindow::paintEvent(QPaintEvent* /*event*/)
{
    // Achtergrond (de plattegrond uit Gebouw.png)
    {
        QPainter painter(this);
        painter.drawPixmap(PNG_OFFSET_X, PNG_OFFSET_Y, _gebouw);
    }

    // Domain-objecten tekenen zichzelf via hun teken-methode.
    _vd.teken(this);
    _d1.teken(this);
    _d2.teken(this);
    _halsensor.teken(this);

    // -------------------------------------------------------------------------
    // DEBUG OVERLAY - rode stippen + labels op huidige scharnier-locaties
    // zodat we kunnen zien waar elk element NU staat. Klik op gewenste plek
    // in het venster en kijk in terminal voor exacte (x, y).
    // Zet kDebug = false om de overlay uit te schakelen.
    // -------------------------------------------------------------------------
    constexpr bool kDebug = true;
    if (kDebug) {
        QPainter dbg(this);
        dbg.setPen(QPen(Qt::red, 2));
        QFont f = dbg.font();
        f.setPointSize(8);
        f.setBold(true);
        dbg.setFont(f);

        auto mark = [&](int x, int y, const QString& label) {
            dbg.drawEllipse(x - 3, y - 3, 6, 6);
            dbg.drawText(x + 7, y + 4,
                         QString("%1 (%2,%3)").arg(label).arg(x).arg(y));
        };
        mark(VD_X, VD_Y, "vd");
        mark(D1_X, D1_Y, "d1");
        mark(D2_X, D2_Y, "d2");
        mark(S1_X, S1_Y, "s1");
    }
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    qInfo().noquote() << QString("CLICK: x=%1 y=%2")
                             .arg(event->pos().x())
                             .arg(event->pos().y());
}

// -----------------------------------------------------------------------------
// Knop-handlers
// -----------------------------------------------------------------------------

void MainWindow::onSchuifdeurKnopClicked()
{
    if (_vd.isDeurOpen()) {
        _vd.sluit();
    } else {
        _vd.open();
    }
    update();
}

void MainWindow::onDraaideurD1KnopClicked()
{
    if (_d1.isDeurOpen()) {
        _d1.sluit();
    } else {
        _d1.open();
    }
    update();
}

void MainWindow::onDraaideurD2KnopClicked()
{
    if (_d2.isDeurOpen()) {
        _d2.sluit();
    } else {
        _d2.open();
    }
    update();
}

void MainWindow::onHalsensorKnopClicked()
{
    if (_halsensor.isGeactiveerd()) {
        _halsensor.deactiveer();
    } else {
        _halsensor.activeer();
    }
    update();
}

} // namespace ui
