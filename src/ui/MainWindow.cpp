/**
 * @file MainWindow.cpp
 * @brief Implementatie van het hoofdvenster.
 */

#include "MainWindow.h"

#include "infra/MockGpio.h"
#include "infra/PigpioGpio.h"

#include <QDebug>
#include <QPainter>
#include <QPushButton>

namespace ui {

// -----------------------------------------------------------------------------
// Constanten
// -----------------------------------------------------------------------------

namespace {
    constexpr int VENSTER_BREEDTE  = 900;
    constexpr int VENSTER_HOOGTE   = 600;
    constexpr int PNG_OFFSET_X     = 20;
    constexpr int PNG_OFFSET_Y     = 20;

    // Schuifdeur (vd) - rechterwand van het gebouw
    constexpr int VD_X       = 506;
    constexpr int VD_Y       = 173;
    constexpr int VD_LENGTE  = 90;

    // Draaideur d1 - verticale wand rechts van kamer 1 (linksboven)
    constexpr int D1_X       = 252;
    constexpr int D1_Y       = 107;
    constexpr int D1_LENGTE  = 35;

    // Draaideur d2 - horizontale wand boven het kamertje rechtsbeneden
    constexpr int D2_X       = 271;
    constexpr int D2_Y       = 302;
    constexpr int D2_LENGTE  = 35;

    // HallSensor (s1)
    constexpr int S1_X       = 524;
    constexpr int S1_Y       = 208;

    // Knoppen-kolom rechts naast het gebouw
    constexpr int BTN_X      = 560;
    constexpr int BTN_BREED  = 150;
    constexpr int BTN_HOOG   = 32;

    // Hardware: BCM pin + servo-hoeken per deur.
    // VD = fysieke servo aangesloten. D1/D2 sturen pulsen naar pinnen
    // waar evt. een servo kan hangen; ontbreekt fysieke servo, dan
    // gebeurt er gewoon niets - de code-flow blijft uniform.
    constexpr int VD_SERVO_PIN    = 18;
    constexpr int VD_HOEK_DICHT   = 0;
    constexpr int VD_HOEK_OPEN    = 90;

    constexpr int D1_SERVO_PIN    = 23;
    constexpr int D1_HOEK_DICHT   = 0;
    constexpr int D1_HOEK_OPEN    = 90;

    constexpr int D2_SERVO_PIN    = 24;
    constexpr int D2_HOEK_DICHT   = 0;
    constexpr int D2_HOEK_OPEN    = 90;

    /**
     * @brief Probeer een echte PigpioGpio te maken; bij falen MockGpio.
     *
     * Zo start de app ook als pigpiod niet draait of we op een ander
     * platform compileren. De fysieke servo werkt dan niet, maar de
     * GUI blijft testbaar.
     */
    std::unique_ptr<infra::IGpio> maakGpio()
    {
        try {
            auto gpio = std::make_unique<infra::PigpioGpio>();
            qInfo() << "PigpioGpio geinitialiseerd - hardware actief";
            return gpio;
        } catch (const infra::PigpioFout& e) {
            qWarning() << "PigpioGpio init faalde:" << e.what()
                       << "- val terug op MockGpio (geen fysieke servo)";
            return std::make_unique<infra::MockGpio>();
        }
    }
}

// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------

MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent)
    , _gebouw(":/assets/Gebouw.png")
    , _gpio(maakGpio())
    , _servoVd(std::make_unique<infra::Servo>(*_gpio, VD_SERVO_PIN,
                                              VD_HOEK_DICHT, VD_HOEK_OPEN))
    , _servoD1(std::make_unique<infra::Servo>(*_gpio, D1_SERVO_PIN,
                                              D1_HOEK_DICHT, D1_HOEK_OPEN))
    , _servoD2(std::make_unique<infra::Servo>(*_gpio, D2_SERVO_PIN,
                                              D2_HOEK_DICHT, D2_HOEK_OPEN))
    , _halsensor(S1_X, S1_Y)
    , _vd(VD_X, VD_Y, VD_LENGTE, &_halsensor)
    , _d1(D1_X, D1_Y, D1_LENGTE,
          domain::Draaideur::Orientatie::VerticaleWand,
          domain::Draaideur::ZwaaiRichting::Negatief)
    , _d2(D2_X, D2_Y, D2_LENGTE,
          domain::Draaideur::Orientatie::HorizontaleWand,
          domain::Draaideur::ZwaaiRichting::Positief)
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

// -----------------------------------------------------------------------------
// paintEvent
// -----------------------------------------------------------------------------

void MainWindow::paintEvent(QPaintEvent* /*event*/)
{
    {
        QPainter painter(this);
        painter.drawPixmap(PNG_OFFSET_X, PNG_OFFSET_Y, _gebouw);
    }

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
        // sluit() kan geweigerd zijn door actieve halsensor;
        // alleen als hij echt dicht is, draaien we de servo terug.
        if (!_vd.isDeurOpen()) {
            _servoVd->zetDicht();
        }
    } else {
        _vd.open();
        _servoVd->zetOpen();
    }
    update();
}

void MainWindow::onDraaideurD1KnopClicked()
{
    if (_d1.isDeurOpen()) {
        _d1.sluit();
        _servoD1->zetDicht();
    } else {
        _d1.open();
        _servoD1->zetOpen();
    }
    update();
}

void MainWindow::onDraaideurD2KnopClicked()
{
    if (_d2.isDeurOpen()) {
        _d2.sluit();
        _servoD2->zetDicht();
    } else {
        _d2.open();
        _servoD2->zetOpen();
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
