/**
 * @file MainWindow.cpp
 * @brief Implementatie van het hoofdvenster - opdracht 1 t/m 6.
 */

#include "MainWindow.h"

#include "domain/CodeSlot.h"
#include "domain/SlotException.h"

#include "infra/MockGpio.h"
#include "infra/PigpioGpio.h"

#include <QCheckBox>
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QPushButton>
#include <QTextBrowser>

namespace ui {

// -----------------------------------------------------------------------------
// Constanten
// -----------------------------------------------------------------------------

namespace {
    constexpr int VENSTER_BREEDTE  = 1000;
    constexpr int VENSTER_HOOGTE   = 700;
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

    constexpr int VD_SERVO_PIN    = 18;
    constexpr int D1_SERVO_PIN    = 23;
    constexpr int D2_SERVO_PIN    = 24;
    constexpr int HOEK_DICHT      = 0;
    constexpr int HOEK_OPEN       = 90;

    // Rode LED die oplicht bij een SlotException (opdracht 6).
    constexpr int LED_ROOD_PIN    = 25;

    // d2 behoudt zijn CodeSlot + HerkenningsSlot
    constexpr int     D2_CODE     = 5678;
    const std::string DEFAULT_NAAM_OK  = "Anna";
    const std::string DEFAULT_NAAM_NEE = "Bert";

    // UI-layout kolom 1 (deur-knoppen)
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
    constexpr int HERKENNING_Y  = 510;

    constexpr int KAARTENBAK_X       = 20;
    constexpr int KAARTENBAK_Y       = 440;
    constexpr int KAARTENBAK_W       = 520;
    constexpr int KAARTENBAK_H       = 240;

    // UI-layout kolom 2 (IdKaart-beheer)
    constexpr int K2_X          = 735;
    constexpr int K2_W          = 250;
    constexpr int K2_INPUT_HOOG = 28;

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

    const QString HERKEN_BTN_STYLE =
        "QPushButton { background-color: #6c4ec7; color: white;"
        "  border: none; border-radius: 4px; padding: 4px;"
        "  font-size: 11px; font-weight: bold; }"
        "QPushButton:hover { background-color: #7e60d3; }";

    const QString TOON_BTN_STYLE =
        "QPushButton { background-color: #444; color: white;"
        "  border: none; border-radius: 4px; padding: 4px;"
        "  font-size: 11px; font-weight: bold; }"
        "QPushButton:hover { background-color: #555; }";

    const QString KAART_BTN_STYLE =
        "QPushButton { background-color: #1a8a8a; color: white;"
        "  border: none; border-radius: 4px; padding: 4px;"
        "  font-size: 11px; font-weight: bold; }"
        "QPushButton:hover { background-color: #20a0a0; }";

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
            qWarning() << "PigpioGpio init faalde:" << e.what();
            return std::make_unique<infra::MockGpio>();
        }
    }

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
} // anonymous namespace

// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------

MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent)
    , _gebouw(":/assets/Gebouw.png")
    , _gpio(maakGpio())
    , _servoVd(std::make_unique<infra::Servo>(*_gpio, VD_SERVO_PIN, HOEK_DICHT, HOEK_OPEN))
    , _servoD1(std::make_unique<infra::Servo>(*_gpio, D1_SERVO_PIN, HOEK_DICHT, HOEK_OPEN))
    , _servoD2(std::make_unique<infra::Servo>(*_gpio, D2_SERVO_PIN, HOEK_DICHT, HOEK_OPEN))
    , _halsensor()
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

    // --- QTextBrowser + Drukbox eerst ---
    _kaartenbakDisplay = new QTextBrowser(this);
    _kaartenbakDisplay->setGeometry(KAARTENBAK_X, KAARTENBAK_Y,
                                    KAARTENBAK_W, KAARTENBAK_H);
    _kaartenbakDisplay->setStyleSheet(
        "QTextBrowser { background-color: #fafafa; border: 1px solid #aaa;"
        " border-radius: 6px; font-family: monospace; font-size: 11pt; }");
    _kaartenbakDisplay->setPlainText(
        "[ Output panel ]\n"
        "Hier verschijnt de kaartenbak van d2 en zoekresultaten van IdKaarten.");

    _drukbox = std::make_unique<Drukbox>(_kaartenbakDisplay);

    // --- Rode exception-LED (opdracht 6) -----------------------------------
    _gpio->configureerOutput(LED_ROOD_PIN);
    _gpio->schrijfDigitaal(LED_ROOD_PIN, false); // begin: LED uit

    // --- Sloten opzetten (opdracht 5: vd en d1 krijgen KaartSlot) ----------
    _kaartSlotVd  = std::make_shared<domain::KaartSlot>("vd");
    _kaartSlotD1  = std::make_shared<domain::KaartSlot>("d1");
    _herkenningD2 = std::make_shared<domain::HerkenningsSlot>(_drukbox.get());
    _herkenningD2->voegAutorissatieToe(DEFAULT_NAAM_OK,  true);
    _herkenningD2->voegAutorissatieToe(DEFAULT_NAAM_NEE, false);

    _slotenVd = { _kaartSlotVd };
    _slotenD1 = { _kaartSlotD1 };
    _slotenD2 = {
        std::make_shared<domain::CodeSlot>(D2_CODE),
        _herkenningD2
    };

    for (auto& s : _slotenVd) _vd.voegSlotToe(s);
    for (auto& s : _slotenD1) _d1.voegSlotToe(s);
    for (auto& s : _slotenD2) _d2.voegSlotToe(s);

    // Standaard kaart in de centrale map zodat user direct kan demoen.
    auto demoKaart = std::make_unique<domain::IdKaart>(
        "K001", "Anna", "Hoofdstraat 1");
    demoKaart->geefToegang(_kaartSlotVd.get());
    demoKaart->geefToegang(_kaartSlotD1.get());
    domain::KaartSlot::voegIdKaartToe(demoKaart.get());
    _idKaartenOwner.push_back(std::move(demoKaart));

    // -------------------------------------------------------------------------
    // Helper: control-stack per deur
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
            _gpio->schrijfDigitaal(LED_ROOD_PIN, false); // reset exception-LED
            for (auto& slot : *slotenPtr) {
                if (!slot) continue;
                // Opdracht 6: een KaartSlot gooit een SlotException bij een
                // mislukte poging. We vangen die hier op (waar ontgrendel
                // wordt aangeroepen), tonen de inhoud en laten de rode LED
                // oplichten.
                try {
                    slot->ontgrendel(invoer);
                } catch (const domain::SlotException& ex) {
                    _gpio->schrijfDigitaal(LED_ROOD_PIN, true);
                    _kaartenbakDisplay->append(
                        QString("[SlotException] plaats '%1' - id: %2")
                            .arg(QString::fromStdString(ex.plaats()),
                                 QString::fromStdString(ex.id())));
                }
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
                   "kaart-id (bv. K001)",
                   &_slotenD1, _inputD1, _statusD1,
                   &MainWindow::onDraaideurD1KnopClicked);

    bouwDeurSectie(VD_SECTIE_Y, "vd (schuifdeur)",
                   "kaart-id (bv. K001)",
                   &_slotenVd, _inputVd, _statusVd,
                   &MainWindow::onSchuifdeurKnopClicked);

    bouwDeurSectie(D2_SECTIE_Y, "d2 (draaideur)",
                   QString("naam of %1").arg(D2_CODE),
                   &_slotenD2, _inputD2, _statusD2,
                   &MainWindow::onDraaideurD2KnopClicked);

    // Halsensor
    auto* btnSens = new QPushButton("halsensor toggle", this);
    btnSens->setGeometry(BTN_X, HALSENSOR_Y, BTN_BREED, BTN_HOOG);
    btnSens->setStyleSheet(SENSOR_BTN_STYLE);
    connect(btnSens, &QPushButton::clicked,
            this, &MainWindow::onHalsensorKnopClicked);

    // --- d2 kaartenbak-beheer (HerkenningsSlot) ---
    auto* lblKaart = new QLabel("Kaartenbak d2:", this);
    lblKaart->setGeometry(BTN_X, HERKENNING_Y, BTN_BREED, 20);
    QFont lblFont = lblKaart->font();
    lblFont.setBold(true);
    lblFont.setPointSize(10);
    lblKaart->setFont(lblFont);

    auto* btnToegang = new QPushButton("+ TOEGANG", this);
    btnToegang->setGeometry(BTN_X, HERKENNING_Y + 24, KLEIN_BTN_W, INPUT_HOOG);
    btnToegang->setStyleSheet(HERKEN_BTN_STYLE);
    connect(btnToegang, &QPushButton::clicked, this, [this]() {
        const auto naam = _inputD2->text().toStdString();
        if (!naam.empty()) {
            _herkenningD2->voegAutorissatieToe(naam, true);
            _inputD2->clear();
        }
    });

    auto* btnGeblokkeerd = new QPushButton("+ GEBLOK.", this);
    btnGeblokkeerd->setGeometry(BTN_X + KLEIN_BTN_W + 5, HERKENNING_Y + 24,
                                KLEIN_BTN_W, INPUT_HOOG);
    btnGeblokkeerd->setStyleSheet(HERKEN_BTN_STYLE);
    connect(btnGeblokkeerd, &QPushButton::clicked, this, [this]() {
        const auto naam = _inputD2->text().toStdString();
        if (!naam.empty()) {
            _herkenningD2->voegAutorissatieToe(naam, false);
            _inputD2->clear();
        }
    });

    auto* btnToon = new QPushButton("Toon kaartenbak", this);
    btnToon->setGeometry(BTN_X, HERKENNING_Y + 24 + INPUT_HOOG + 4,
                         BTN_BREED, INPUT_HOOG);
    btnToon->setStyleSheet(TOON_BTN_STYLE);
    connect(btnToon, &QPushButton::clicked, this, [this]() {
        _herkenningD2->toonKaartenbak();
    });

    // -------------------------------------------------------------------------
    // KOLOM 2 - IdKaart-beheer (opdracht 5)
    // -------------------------------------------------------------------------
    auto* lblKaartBeheer = new QLabel("IdKaart-beheer (vd + d1)", this);
    lblKaartBeheer->setGeometry(K2_X, 30, K2_W, 22);
    QFont k2Font = lblKaartBeheer->font();
    k2Font.setBold(true);
    k2Font.setPointSize(11);
    lblKaartBeheer->setFont(k2Font);
    lblKaartBeheer->setStyleSheet("color: #1a8a8a;");

    _inputKaartId = new QLineEdit(this);
    _inputKaartId->setGeometry(K2_X, 60, K2_W, K2_INPUT_HOOG);
    _inputKaartId->setPlaceholderText("Kaart-id (bv. K002)");
    _inputKaartId->setStyleSheet(INPUT_STYLE);

    _inputKaartNaam = new QLineEdit(this);
    _inputKaartNaam->setGeometry(K2_X, 92, K2_W, K2_INPUT_HOOG);
    _inputKaartNaam->setPlaceholderText("Naam eigenaar");
    _inputKaartNaam->setStyleSheet(INPUT_STYLE);

    _inputKaartAdres = new QLineEdit(this);
    _inputKaartAdres->setGeometry(K2_X, 124, K2_W, K2_INPUT_HOOG);
    _inputKaartAdres->setPlaceholderText("Adres eigenaar");
    _inputKaartAdres->setStyleSheet(INPUT_STYLE);

    _cbToegangVd = new QCheckBox("Toegang vd", this);
    _cbToegangVd->setGeometry(K2_X, 158, 120, 22);
    _cbToegangVd->setChecked(true);

    _cbToegangD1 = new QCheckBox("Toegang d1", this);
    _cbToegangD1->setGeometry(K2_X + 125, 158, 120, 22);
    _cbToegangD1->setChecked(true);

    auto* btnMaak = new QPushButton("+ Maak IdKaart", this);
    btnMaak->setGeometry(K2_X, 188, K2_W, INPUT_HOOG + 2);
    btnMaak->setStyleSheet(KAART_BTN_STYLE);
    connect(btnMaak, &QPushButton::clicked,
            this, &MainWindow::onMaakIdKaartClicked);

    _inputZoekVerw = new QLineEdit(this);
    _inputZoekVerw->setGeometry(K2_X, 230, K2_W, K2_INPUT_HOOG);
    _inputZoekVerw->setPlaceholderText("kaart-id voor verwijderen / naam voor zoek");
    _inputZoekVerw->setStyleSheet(INPUT_STYLE);

    auto* btnVerwijder = new QPushButton("Verwijder", this);
    btnVerwijder->setGeometry(K2_X, 262, 80, INPUT_HOOG);
    btnVerwijder->setStyleSheet(VERGR_BTN_STYLE);
    connect(btnVerwijder, &QPushButton::clicked,
            this, &MainWindow::onVerwijderIdKaartClicked);

    auto* btnZoek = new QPushButton("Zoek (naam)", this);
    btnZoek->setGeometry(K2_X + 85, 262, 80, INPUT_HOOG);
    btnZoek->setStyleSheet(KAART_BTN_STYLE);
    connect(btnZoek, &QPushButton::clicked,
            this, &MainWindow::onZoekOpNaamClicked);

    auto* btnToonAlle = new QPushButton("Toon alle", this);
    btnToonAlle->setGeometry(K2_X + 170, 262, 80, INPUT_HOOG);
    btnToonAlle->setStyleSheet(TOON_BTN_STYLE);
    connect(btnToonAlle, &QPushButton::clicked,
            this, &MainWindow::onToonAlleKaartenClicked);
}

// -----------------------------------------------------------------------------
// Destructor - leeg de statische map zodat dangling pointers daar geen
// problemen geven bij eventuele latere KaartSlot::-aanroepen.
// -----------------------------------------------------------------------------

MainWindow::~MainWindow()
{
    domain::KaartSlot::wisAlleIdKaarten();
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

    // Schuifdeur (vd): verticale lijn als dicht, niets als open.
    if (!_vd.isDeurOpen()) {
        QPainter painter(this);
        const QColor kleur = (!_vd.alleSlotenOntgrendeld())
                           ? QColor(220, 50, 50) : Qt::black;
        painter.setPen(QPen(kleur, 5, Qt::SolidLine, Qt::FlatCap));
        painter.drawLine(VD_X, VD_Y, VD_X, VD_Y + VD_LENGTE);
    }

    // Draaideur d1 (VerticaleWand, Negatief): dicht = verticaal, open = horizontaal naar links.
    {
        QPainter painter(this);
        const QColor kleur = (!_d1.alleSlotenOntgrendeld())
                           ? QColor(220, 50, 50) : Qt::black;
        painter.setPen(QPen(kleur, 5, Qt::SolidLine, Qt::FlatCap));
        if (_d1.isLiggend()) {
            painter.drawLine(D1_X, D1_Y, D1_X - D1_LENGTE, D1_Y);
        } else {
            painter.drawLine(D1_X, D1_Y, D1_X, D1_Y + D1_LENGTE);
        }
    }

    // Draaideur d2 (HorizontaleWand, Positief): dicht = horizontaal, open = verticaal naar onder.
    {
        QPainter painter(this);
        const QColor kleur = (!_d2.alleSlotenOntgrendeld())
                           ? QColor(220, 50, 50) : Qt::black;
        painter.setPen(QPen(kleur, 5, Qt::SolidLine, Qt::FlatCap));
        if (_d2.isLiggend()) {
            painter.drawLine(D2_X, D2_Y, D2_X, D2_Y + D2_LENGTE);
        } else {
            painter.drawLine(D2_X, D2_Y, D2_X + D2_LENGTE, D2_Y);
        }
    }

    // Halsensor: cirkel geel (rust) of blauw (geactiveerd).
    {
        QPainter painter(this);
        QColor kleur = _halsensor.isGeactiveerd() ? Qt::blue : Qt::yellow;
        painter.setBrush(QBrush(kleur, Qt::SolidPattern));
        painter.setPen(QPen(kleur, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawEllipse(S1_X, S1_Y, 18, 18);
    }

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
// Deur-handlers
// -----------------------------------------------------------------------------

void MainWindow::onSchuifdeurKnopClicked()
{
    if (_vd.isDeurOpen()) {
        _vd.sluit();
        if (!_vd.isDeurOpen()) _servoVd->zetDicht();
    } else {
        _vd.open();
        if (_vd.isDeurOpen()) _servoVd->zetOpen();
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
        if (_d1.isDeurOpen()) _servoD1->zetOpen();
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
        if (_d2.isDeurOpen()) _servoD2->zetOpen();
    }
    updateSlotStatusLabels();
    update();
}

void MainWindow::onHalsensorKnopClicked()
{
    if (_halsensor.isGeactiveerd()) _halsensor.deactiveer();
    else                            _halsensor.activeer();
    update();
}

void MainWindow::updateSlotStatusLabels()
{
    zetStatusLabel(_statusVd, _slotenVd);
    zetStatusLabel(_statusD1, _slotenD1);
    zetStatusLabel(_statusD2, _slotenD2);
}

// -----------------------------------------------------------------------------
// IdKaart-handlers (opdracht 5)
// -----------------------------------------------------------------------------

void MainWindow::onMaakIdKaartClicked()
{
    const auto id    = _inputKaartId->text().toStdString();
    const auto naam  = _inputKaartNaam->text().toStdString();
    const auto adres = _inputKaartAdres->text().toStdString();

    if (id.empty() || naam.empty()) {
        _drukbox->clearMedium();
        _drukbox->toonText("[ FOUT ] id en naam moeten ingevuld zijn.");
        return;
    }

    auto kaart = std::make_unique<domain::IdKaart>(id, naam, adres);
    if (_cbToegangVd->isChecked()) kaart->geefToegang(_kaartSlotVd.get());
    if (_cbToegangD1->isChecked()) kaart->geefToegang(_kaartSlotD1.get());

    domain::KaartSlot::voegIdKaartToe(kaart.get());
    _idKaartenOwner.push_back(std::move(kaart));

    _inputKaartId->clear();
    _inputKaartNaam->clear();
    _inputKaartAdres->clear();

    _drukbox->clearMedium();
    _drukbox->toonText(QString("[OK] kaart '%1' aangemaakt").arg(
        QString::fromStdString(id)).toStdString());
}

void MainWindow::onVerwijderIdKaartClicked()
{
    const auto id = _inputZoekVerw->text().toStdString();
    if (id.empty()) {
        _drukbox->clearMedium();
        _drukbox->toonText("[ FOUT ] vul kaart-id in om te verwijderen");
        return;
    }

    domain::KaartSlot::verwijderIdKaart(id);

    // Verwijder ook de owner zodat memory vrijkomt.
    _idKaartenOwner.erase(
        std::remove_if(_idKaartenOwner.begin(), _idKaartenOwner.end(),
            [&id](const std::unique_ptr<domain::IdKaart>& k) {
                return k->userId() == id;
            }),
        _idKaartenOwner.end());

    _inputZoekVerw->clear();
    _drukbox->clearMedium();
    _drukbox->toonText(QString("[OK] kaart '%1' verwijderd (indien aanwezig)").arg(
        QString::fromStdString(id)).toStdString());
}

void MainWindow::onZoekOpNaamClicked()
{
    const auto naam = _inputZoekVerw->text().toStdString();
    if (naam.empty()) {
        _drukbox->clearMedium();
        _drukbox->toonText("[ FOUT ] vul naam in om op te zoeken");
        return;
    }

    // **Lambda calculus** in actie: predicate-functie wordt door
    // zoekIdKaarten gebruikt om de statische map te filteren.
    auto resultaten = domain::KaartSlot::zoekIdKaarten(
        [&naam](const domain::IdKaart& k) {
            return k.naamEigenaar() == naam;
        });

    _drukbox->clearMedium();
    _drukbox->toonText("=== Zoekresultaat op naam ===");
    if (resultaten.empty()) {
        _drukbox->toonText(QString("Geen kaarten gevonden voor '%1'.").arg(
            QString::fromStdString(naam)).toStdString());
    } else {
        for (auto* k : resultaten) {
            _drukbox->toonText(k->userId() + " : " + k->naamEigenaar()
                             + " (" + k->adresEigenaar() + ")");
        }
    }
}

void MainWindow::onToonAlleKaartenClicked()
{
    auto alle = domain::KaartSlot::zoekIdKaarten(
        [](const domain::IdKaart&) { return true; }); // altijd-true lambda

    _drukbox->clearMedium();
    _drukbox->toonText("=== Alle geregistreerde IdKaarten ===");
    if (alle.empty()) {
        _drukbox->toonText("(leeg)");
    } else {
        for (auto* k : alle) {
            _drukbox->toonText(k->userId() + " : " + k->naamEigenaar()
                             + " (" + k->adresEigenaar() + ")");
        }
    }
}

} // namespace ui
