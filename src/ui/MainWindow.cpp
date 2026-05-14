/**
 * @file MainWindow.cpp
 * @brief Implementatie van het hoofdvenster - opdracht 1 + 2.
 */

#include "MainWindow.h"

#include "domain/CodeSlot.h"
#include "domain/SleutelSlot.h"

#include "infra/MockGpio.h"
#include "infra/PigpioGpio.h"

#include <QDebug>
#include <QLabel>
#include <QLineEdit>
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

    // Hardware: BCM pin + servo-hoeken per deur.
    constexpr int VD_SERVO_PIN    = 18;
    constexpr int VD_HOEK_DICHT   = 0;
    constexpr int VD_HOEK_OPEN    = 90;

    constexpr int D1_SERVO_PIN    = 23;
    constexpr int D1_HOEK_DICHT   = 0;
    constexpr int D1_HOEK_OPEN    = 90;

    constexpr int D2_SERVO_PIN    = 24;
    constexpr int D2_HOEK_DICHT   = 0;
    constexpr int D2_HOEK_OPEN    = 90;

    // Sleutels en codes - per deur
    const std::string VD_SLEUTEL = "vdsleutel";
    constexpr int     D1_CODE    = 1234;
    constexpr int     D2_CODE    = 5678;

    // UI-layout: rechter kolom met deur-controls
    constexpr int BTN_X         = 560;
    constexpr int BTN_BREED     = 130;
    constexpr int BTN_HOOG      = 28;
    constexpr int INPUT_HOOG    = 24;
    constexpr int KLEIN_BTN_W   = 62;
    constexpr int STATUS_HOOG   = 22;

    // Per deur 4 widgets verticaal gestapeld (~120 px hoog totaal)
    constexpr int D1_SECTIE_Y   = 40;
    constexpr int VD_SECTIE_Y   = 170;
    constexpr int D2_SECTIE_Y   = 300;
    constexpr int HALSENSOR_Y   = 450;

    /// Maakt een PigpioGpio of valt terug op MockGpio.
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

    /// Hulp: zet status-label tekst + kleur op basis van slot-staat.
    void zetStatusLabel(QLabel* label, const domain::Slot& slot)
    {
        if (slot.isVergrendeld()) {
            label->setText("[X] vergrendeld");
            label->setStyleSheet("color: red;");
        } else {
            label->setText("[ ] ontgrendeld");
            label->setStyleSheet("color: green;");
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
    , _slotVd(std::make_shared<domain::SleutelSlot>(VD_SLEUTEL))
    , _slotD1(std::make_shared<domain::CodeSlot>(D1_CODE))
    , _slotD2(std::make_shared<domain::CodeSlot>(D2_CODE))
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

    // Koppel sloten aan deuren.
    _vd.setSlot(_slotVd);
    _d1.setSlot(_slotD1);
    _d2.setSlot(_slotD2);

    // -------------------------------------------------------------------------
    // Helper-lambda om voor een deur een hele control-stack te bouwen
    // (deur-knop + input + ontgrendel + vergrendel + status).
    // -------------------------------------------------------------------------
    auto bouwDeurSectie = [this](int yOffset,
                                  const QString& deurLabel,
                                  const QString& placeholder,
                                  std::shared_ptr<domain::Slot> slot,
                                  QLineEdit*& inputOut,
                                  QLabel*& statusOut,
                                  void (MainWindow::*deurClick)())
    {
        auto* btnDeur = new QPushButton(deurLabel, this);
        btnDeur->setGeometry(BTN_X, yOffset, BTN_BREED, BTN_HOOG);
        connect(btnDeur, &QPushButton::clicked, this, deurClick);

        inputOut = new QLineEdit(this);
        inputOut->setGeometry(BTN_X, yOffset + 34, BTN_BREED, INPUT_HOOG);
        inputOut->setPlaceholderText(placeholder);

        auto* btnOntgr = new QPushButton("Ontgrendel", this);
        btnOntgr->setGeometry(BTN_X, yOffset + 62, KLEIN_BTN_W, INPUT_HOOG);
        connect(btnOntgr, &QPushButton::clicked, this, [this, slot, inputOut]() {
            slot->ontgrendel(inputOut->text().toStdString());
            updateSlotStatusLabels();
        });

        auto* btnVergr = new QPushButton("Vergrendel", this);
        btnVergr->setGeometry(BTN_X + KLEIN_BTN_W + 6, yOffset + 62,
                              KLEIN_BTN_W, INPUT_HOOG);
        connect(btnVergr, &QPushButton::clicked, this, [this, slot]() {
            slot->vergrendel();
            updateSlotStatusLabels();
        });

        statusOut = new QLabel(this);
        statusOut->setGeometry(BTN_X, yOffset + 92, BTN_BREED, STATUS_HOOG);
        zetStatusLabel(statusOut, *slot);
    };

    bouwDeurSectie(D1_SECTIE_Y, "d1 (draaideur)", "code: 1234",
                   _slotD1, _inputD1, _statusD1,
                   &MainWindow::onDraaideurD1KnopClicked);

    bouwDeurSectie(VD_SECTIE_Y, "vd (schuifdeur)", "sleutel: vdsleutel",
                   _slotVd, _inputVd, _statusVd,
                   &MainWindow::onSchuifdeurKnopClicked);

    bouwDeurSectie(D2_SECTIE_Y, "d2 (draaideur)", "code: 5678",
                   _slotD2, _inputD2, _statusD2,
                   &MainWindow::onDraaideurD2KnopClicked);

    // Halsensor - geen slot, alleen toggle-knop.
    auto* btnSens = new QPushButton("halsensor toggle", this);
    btnSens->setGeometry(BTN_X, HALSENSOR_Y, BTN_BREED, BTN_HOOG);
    connect(btnSens, &QPushButton::clicked,
            this, &MainWindow::onHalsensorKnopClicked);
}

// -----------------------------------------------------------------------------
// Tekenen
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
// Handlers - deur-knoppen synchroniseren servo met domain-staat.
// Wanneer slot vergrendeld is, blijft domain status onveranderd
// (Deur::open weigert). Dan veranderen we de servo ook niet.
// -----------------------------------------------------------------------------

void MainWindow::onSchuifdeurKnopClicked()
{
    if (_vd.isDeurOpen()) {
        _vd.sluit();
        if (!_vd.isDeurOpen()) {
            _servoVd->zetDicht();
        }
    } else {
        _vd.open();
        if (_vd.isDeurOpen()) {
            _servoVd->zetOpen();
        }
    }
    updateSlotStatusLabels(); // slot vergrendelt bij sluit() - status bijwerken
    update();
}

void MainWindow::onDraaideurD1KnopClicked()
{
    if (_d1.isDeurOpen()) {
        _d1.sluit();
        _servoD1->zetDicht();
    } else {
        _d1.open();
        if (_d1.isDeurOpen()) {
            _servoD1->zetOpen();
        }
    }
    updateSlotStatusLabels();
    update();
}

void MainWindow::onDraaideurD2KnopClicked()
{
    if (_d2.isDeurOpen()) {
        _d2.sluit();
        _servoD2->zetDicht();
    } else {
        _d2.open();
        if (_d2.isDeurOpen()) {
            _servoD2->zetOpen();
        }
    }
    updateSlotStatusLabels();
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

// -----------------------------------------------------------------------------
// Helper
// -----------------------------------------------------------------------------

void MainWindow::updateSlotStatusLabels()
{
    zetStatusLabel(_statusVd, *_slotVd);
    zetStatusLabel(_statusD1, *_slotD1);
    zetStatusLabel(_statusD2, *_slotD2);
}

} // namespace ui
