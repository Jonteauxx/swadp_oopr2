/**
 * @file MainWindow.h
 * @brief Hoofdvenster van de GebouwBeheer-applicatie (opdracht 1 t/m 5).
 * @author tj.herdigein
 * @date 2026
 */

#ifndef SWADP_OOPR2_MAINWINDOW_H
#define SWADP_OOPR2_MAINWINDOW_H

#include "domain/Draaideur.h"
#include "infra/HallSensor.h"
#include "domain/HerkenningsSlot.h"
#include "domain/IdKaart.h"
#include "domain/KaartSlot.h"
#include "domain/Schuifdeur.h"
#include "domain/Slot.h"

#include "infra/IGpio.h"
#include "infra/Servo.h"

#include "ui/Drukbox.h"

#include <QPixmap>
#include <QWidget>

#include <memory>
#include <vector>

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTextBrowser;
class QTimer;

namespace ui {

/**
 * @class MainWindow
 * @brief Hoofdvenster - plattegrond, hardware, sloten, kaartenbak,
 *        en IdKaart-beheer (opdracht 5).
 *
 * Opdracht 5-uitbreiding:
 *   - vd en d1 krijgen een KaartSlot (centrale kaartenadministratie).
 *   - d2 behoudt CodeSlot + HerkenningsSlot van opdracht 4.
 *   - Rechts in het venster komt een paneel om IdKaarten aan te maken,
 *     toegang te configureren, te verwijderen en op naam te zoeken.
 *   - Het zoeken op naam gebruikt **lambda calculus** in
 *     KaartSlot::zoekIdKaarten().
 */
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void onSchuifdeurKnopClicked();
    void onDraaideurD1KnopClicked();
    void onDraaideurD2KnopClicked();
    void onHalsensorKnopClicked();

    /// Leest periodiek de fysieke drukknoppen (GPIO-input) en roept bij een
    /// druk (vallende flank, pull-up) de bijbehorende deur/sensor-actie aan.
    void pollKnoppen();

private:
    void updateSlotStatusLabels();

    /// Maak een nieuwe IdKaart op basis van de UI-invoer + checkboxes,
    /// registreer in de centrale map en behoud eigendom.
    void onMaakIdKaartClicked();

    /// Verwijder de IdKaart met de id uit de zoek/verwijder-input.
    void onVerwijderIdKaartClicked();

    /// Zoek IdKaarten met naam == zoek-input en toon ze via Drukbox.
    void onZoekOpNaamClicked();

    /// Toon ALLE geregistreerde IdKaarten via Drukbox.
    void onToonAlleKaartenClicked();

    QPixmap _gebouw;

    // Hardware
    std::unique_ptr<infra::IGpio> _gpio;
    std::unique_ptr<infra::Servo> _servoVd;
    std::unique_ptr<infra::Servo> _servoD1;
    std::unique_ptr<infra::Servo> _servoD2;

    // Fysieke drukknoppen: timer + vorige staat per knop (pull-up: true = los).
    QTimer* _knopTimer = nullptr;
    bool _vorigeKnopVd  = true;
    bool _vorigeKnopD1  = true;
    bool _vorigeKnopD2  = true;
    bool _vorigeKnopHal = true;

    // Drukbox/QTextBrowser
    QTextBrowser* _kaartenbakDisplay = nullptr;
    std::unique_ptr<Drukbox> _drukbox;

    // Sloten per deur
    std::vector<std::shared_ptr<domain::Slot>> _slotenVd;
    std::vector<std::shared_ptr<domain::Slot>> _slotenD1;
    std::vector<std::shared_ptr<domain::Slot>> _slotenD2;
    std::shared_ptr<domain::KaartSlot>        _kaartSlotVd;
    std::shared_ptr<domain::KaartSlot>        _kaartSlotD1;
    std::shared_ptr<domain::HerkenningsSlot>  _herkenningD2;

    // IdKaart-eigendom (in centrale map gehouden als raw pointers).
    std::vector<std::unique_ptr<domain::IdKaart>> _idKaartenOwner;

    // Domain
    infra::HallSensor _halsensor;
    domain::Schuifdeur _vd;
    domain::Draaideur  _d1;
    domain::Draaideur  _d2;

    // Deur-UI
    QLineEdit*   _inputVd      = nullptr;
    QLineEdit*   _inputD1      = nullptr;
    QLineEdit*   _inputD2      = nullptr;
    QLabel*      _statusVd     = nullptr;
    QLabel*      _statusD1     = nullptr;
    QLabel*      _statusD2     = nullptr;

    // IdKaart-beheer-UI (kolom 2)
    QLineEdit*   _inputKaartId    = nullptr;
    QLineEdit*   _inputKaartNaam  = nullptr;
    QLineEdit*   _inputKaartAdres = nullptr;
    QCheckBox*   _cbToegangVd     = nullptr;
    QCheckBox*   _cbToegangD1     = nullptr;
    QLineEdit*   _inputZoekVerw   = nullptr;
};

} // namespace ui

#endif // SWADP_OOPR2_MAINWINDOW_H
