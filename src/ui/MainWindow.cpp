/**
 * @file MainWindow.cpp
 * @brief Implementatie van het hoofdvenster.
 */

#include "MainWindow.h"

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
    constexpr int VD_X       = 500;
    constexpr int VD_Y       = 195;
    constexpr int VD_LENGTE  = 55;

    // Draaideur d1 - wand rechts van kamer 1 (linksboven)
    constexpr int D1_X       = 200;
    constexpr int D1_Y       = 95;
    constexpr int D1_LENGTE  = 35;

    // Draaideur d2 - wand links van kamertje rechtsonder
    constexpr int D2_X       = 310;
    constexpr int D2_Y       = 260;
    constexpr int D2_LENGTE  = 30;

    // HallSensor (s1) - rechts naast de schuifdeur, buiten het gebouw
    constexpr int S1_X       = 530;
    constexpr int S1_Y       = 215;

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
    // Elk maakt zijn eigen QPainter zodat we niet hoeven door te geven.
    _vd.teken(this);
    _d1.teken(this);
    _d2.teken(this);
    _halsensor.teken(this);
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
