/**
 * @file MainWindow.cpp
 * @brief Implementatie van het hoofdvenster - opdracht 1 t/m 3.
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

    constexpr int VD_X       = 506;
    constexpr int VD_Y       = 173;
    constexpr int VD_LENGTE  = 90;

    constexpr int D1_X       = 252;
    constexpr int D1_Y       = 107;
    constexpr int D1_LENGTE  = 35;

    constexpr int D2_X       = 271;
    constexpr int D2_Y       = 302;
    constexpr int D2_LENGTE  = 35;

    constexpr int S1_X       = 524;
    constexpr int S1_Y       = 208;

    // Hardware: BCM pin + servo-hoeken
    constexpr int VD_SERVO_PIN    = 18;
    constexpr int D1_SERVO_PIN    = 23;
    constexpr int D2_SERVO_PIN    = 24;
    constexpr int HOEK_DICHT      = 0;
    constexpr int HOEK_OPEN       = 90;

    // Sleutels en codes (per opdracht 3: minimaal één deur met 2 sloten)
    const std::string VD_SLEUTEL  = "vdsleutel";
    constexpr int     VD_CODE     = 1111;
    constexpr int     D1_CODE     = 1234;
    const std::string D2_SLEUTEL  = "d2sleutel";
    constexpr int     D2_CODE     = 5678;

    // UI-layout
    constexpr int BTN_X         = 555;
    constexpr int BTN_BREED     = 165;
    constexpr int BTN_HOOG      = 34;
    constexpr int INPUT_HOOG    = 28;
    constexpr int KLEIN_BTN_W   = 80;
    constexpr int STATUS_HOOG   = 24;

    constexpr int D1_SECTIE_Y   = 30;
    constexpr int VD_SECTIE_Y   = 170;
    constexpr int D2_SECTIE_Y   = 310;
    constexpr int HALSENSOR_Y   = 460;

    const QString DEUR_BTN_STYLE =
        "QPushButton { background-color: #2c5aa0; color: white;"
        "  border: none; border-radius: 5px; padding: 6px;"
        "  font-size: 12px; font-weight: bold; }"
        "QPushButton:hover { background-color: #3a6cb8; }"
        "QPushButton:pressed { background-color: #1f4575; }";

    const QString ONTGR_BTN_STYLE =
        "QPushButton { background-color: #2e8b57; color: white;"
        "  border: none; border-radius: 4px; padding: 4px;"
        "  font-size: 11px; font-weight: bold; }"
        "QPushButton:hover { background-color: #3aa66a; }";

    const QString VERGR_BTN_STYLE =
        "QPushButton { background-color: #b04545; color: white;"
        "  border: none; border-radius: 4px; padding: 4px;"
        "  font-size: 11px; font-weight: bold; }"
        "QPushButton:hover { background-color: #c75555; }";

    const QString SENSOR_BTN_STYLE =
        "QPushButton { background-color: #d9a043; color: white;"
        "  border: none; border-radius: 5px; padding: 6px;"
        "  font-size: 12px; font-weight: bold; }"
        "QPushButton:hover { background-color: #e6b057; }";

    const QString INPUT_STYLE =
        "QLineEdit { border: 1px solid #999; border-radius: 4px;"
        "  padding: 4px; font-size: 12px; }";

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

    /// Update een status-label met "X/Y vergrendeld" + kleur.
    void zetStatusLabel(QLabel* label,
                        const std::vector<std::shared_ptr<domain::Slot>>& sloten)
    {
        std::size_t v = 0;
        for (const auto& s : sloten) {
            if (s && s->isVergrendeld()) ++v;
        }
        const std::size_t totaal = sloten.size();

        if (totaal == 0) {
            label->setText("geen sloten");
            label->setStyleSheet("color: gray;");
        } else if (v == 0) {
            label->setText(QString("0/%1 vergrendeld - klaar").arg(totaal));
            label->setStyleSheet("color: green;");
        } else {
            label->setText(QString("%1/%2 vergrendeld").arg(v).arg(totaal));
            label->setStyleSheet("color: red;");
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
                                              HOEK_DICHT, HOEK_OPEN))
    , _servoD1(std::make_unique<infra::Servo>(*_gpio, D1_SERVO_PIN,
                                              HOEK_DICHT, HOEK_OPEN))
    , _servoD2(std::make_unique<infra::Servo>(*_gpio, D2_SERVO_PIN,
                                              HOEK_DICHT, HOEK_OPEN))
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

    // Sloten aanmaken volgens opdracht 3:
    //   - vd  : SleutelSlot + CodeSlot (twee sloten)
    //   - d1  : CodeSlot (één slot, conform "een andere deur 1 Slot-object")
    //   - d2  : SleutelSlot + CodeSlot (twee sloten van verschillend type)
    _slotenVd = {
        std::make_shared<domain::SleutelSlot>(VD_SLEUTEL),
        std::make_shared<domain::CodeSlot>(VD_CODE)
    };
    _slotenD1 = {
        std::make_shared<domain::CodeSlot>(D1_CODE)
    };
    _slotenD2 = {
        std::make_shared<domain::SleutelSlot>(D2_SLEUTEL),
        std::make_shared<domain::CodeSlot>(D2_CODE)
    };

    // Koppel elk slot aan zijn deur.
    for (auto& s : _slotenVd) _vd.voegSlotToe(s);
    for (auto& s : _slotenD1) _d1.voegSlotToe(s);
    for (auto& s : _slotenD2) _d2.voegSlotToe(s);

    // -------------------------------------------------------------------------
    // Helper-lambda om voor een deur een hele control-stack te bouwen.
    // Eén invoerveld + één Ontgrendel-knop die ALLE sloten van die deur
    // probeert te openen met dezelfde input - user kan dus eerst sleutel
    // intypen + klikken (eerste slot opent), dan code + klikken (tweede opent).
    // -------------------------------------------------------------------------
    auto bouwDeurSectie = [this](int yOffset,
                                  const QString& deurLabel,
                                  const QString& placeholder,
                                  std::vector<std::shared_ptr<domain::Slot>>* slotenPtr,
                                  QLineEdit*& inputOut,
                                  QLabel*& statusOut,
                                  void (MainWindow::*deurClick)())
    {
        auto* btnDeur = new QPushButton(deurLabel, this);
        btnDeur->setGeometry(BTN_X, yOffset, BTN_BREED, BTN_HOOG);
        btnDeur->setStyleSheet(DEUR_BTN_STYLE);
        connect(btnDeur, &QPushButton::clicked, this, deurClick);

        inputOut = new QLineEdit(this);
        inputOut->setGeometry(BTN_X, yOffset + 40, BTN_BREED, INPUT_HOOG);
        inputOut->setPlaceholderText(placeholder);
        inputOut->setStyleSheet(INPUT_STYLE);

        auto* btnOntgr = new QPushButton("Ontgrendel", this);
        btnOntgr->setGeometry(BTN_X, yOffset + 74, KLEIN_BTN_W, INPUT_HOOG);
        btnOntgr->setStyleSheet(ONTGR_BTN_STYLE);
        connect(btnOntgr, &QPushButton::clicked, this,
                [this, slotenPtr, inputOut]() {
            const std::string invoer = inputOut->text().toStdString();
            for (auto& slot : *slotenPtr) {
                if (slot) slot->ontgrendel(invoer);
            }
            updateSlotStatusLabels();
            update();
        });

        auto* btnVergr = new QPushButton("Vergrendel", this);
        btnVergr->setGeometry(BTN_X + KLEIN_BTN_W + 5, yOffset + 74,
                              KLEIN_BTN_W, INPUT_HOOG);
        btnVergr->setStyleSheet(VERGR_BTN_STYLE);
        connect(btnVergr, &QPushButton::clicked, this,
                [this, slotenPtr]() {
            for (auto& slot : *slotenPtr) {
                if (slot) slot->vergrendel();
            }
            updateSlotStatusLabels();
            update();
        });

        statusOut = new QLabel(this);
        statusOut->setGeometry(BTN_X, yOffset + 106, BTN_BREED, STATUS_HOOG);
        QFont statusFont = statusOut->font();
        statusFont.setPointSize(10);
        statusFont.setBold(true);
        statusOut->setFont(statusFont);
        zetStatusLabel(statusOut, *slotenPtr);
    };

    bouwDeurSectie(D1_SECTIE_Y, "d1 (draaideur)",
                   QString("code: %1").arg(D1_CODE),
                   &_slotenD1, _inputD1, _statusD1,
                   &MainWindow::onDraaideurD1KnopClicked);

    bouwDeurSectie(VD_SECTIE_Y, "vd (schuifdeur)",
                   QString("'%1' of %2").arg(QString::fromStdString(VD_SLEUTEL))
                                        .arg(VD_CODE),
                   &_slotenVd, _inputVd, _statusVd,
                   &MainWindow::onSchuifdeurKnopClicked);

    bouwDeurSectie(D2_SECTIE_Y, "d2 (draaideur)",
                   QString("'%1' of %2").arg(QString::fromStdString(D2_SLEUTEL))
                                        .arg(D2_CODE),
                   &_slotenD2, _inputD2, _statusD2,
                   &MainWindow::onDraaideurD2KnopClicked);

    auto* btnSens = new QPushButton("halsensor toggle", this);
    btnSens->setGeometry(BTN_X, HALSENSOR_Y, BTN_BREED, BTN_HOOG);
    btnSens->setStyleSheet(SENSOR_BTN_STYLE);
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

    // Labels op de plattegrond.
    {
        QPainter painter(this);
        QFont labelFont;
        labelFont.setPointSize(13);
        labelFont.setBold(true);
        painter.setFont(labelFont);
        painter.setPen(QColor(60, 90, 160));

        painter.drawText(D1_X + 12, D1_Y + 10, "d1");
        painter.drawText(D2_X + 12, D2_Y - 5,  "d2");
        painter.drawText(VD_X + 18, VD_Y + 45, "vd");

        QFont sensorFont;
        sensorFont.setPointSize(10);
        sensorFont.setBold(true);
        painter.setFont(sensorFont);
        painter.setPen(QColor(120, 100, 0));
        painter.drawText(S1_X - 30, S1_Y + 35, "halsensor");
    }
}

// -----------------------------------------------------------------------------
// Handlers
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
    updateSlotStatusLabels();
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

void MainWindow::updateSlotStatusLabels()
{
    zetStatusLabel(_statusVd, _slotenVd);
    zetStatusLabel(_statusD1, _slotenD1);
    zetStatusLabel(_statusD2, _slotenD2);
}

} // namespace ui
