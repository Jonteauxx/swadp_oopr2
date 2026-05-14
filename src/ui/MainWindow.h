/**
 * @file MainWindow.h
 * @brief Hoofdvenster van de GebouwBeheer-applicatie (opdracht 1 t/m 4).
 * @author tj.herdigein
 * @date 2026
 */

#ifndef SWADP_OOPR2_MAINWINDOW_H
#define SWADP_OOPR2_MAINWINDOW_H

#include "domain/Draaideur.h"
#include "domain/HallSensor.h"
#include "domain/HerkenningsSlot.h"
#include "domain/Schuifdeur.h"
#include "domain/Slot.h"

#include "infra/IGpio.h"
#include "infra/Servo.h"

#include "ui/Drukbox.h"

#include <QPixmap>
#include <QWidget>

#include <memory>
#include <vector>

class QLabel;
class QLineEdit;
class QPushButton;
class QTextBrowser;

namespace ui {

/**
 * @class MainWindow
 * @brief Plattegrond, hardware-aansturing en UI voor (ont)grendelen +
 *        kaartenbak-beheer (opdracht 4).
 *
 * Opdracht 4-uitbreiding:
 *   - d2 (draaideur) krijgt twee verschillende sloten: een CodeSlot
 *     EN een HerkenningsSlot.
 *   - Onder de plattegrond verschijnt een QTextBrowser waar de
 *     inhoud van de kaartenbak getoond wordt.
 *   - Extra knoppen om iemand toe te voegen (TOEGANG of GEBLOKKEERD).
 */
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void onSchuifdeurKnopClicked();
    void onDraaideurD1KnopClicked();
    void onDraaideurD2KnopClicked();
    void onHalsensorKnopClicked();

private:
    void updateSlotStatusLabels();

    QPixmap _gebouw;

    // Hardware
    std::unique_ptr<infra::IGpio> _gpio;
    std::unique_ptr<infra::Servo> _servoVd;
    std::unique_ptr<infra::Servo> _servoD1;
    std::unique_ptr<infra::Servo> _servoD2;

    // Drukbox = Afdrukker-impl voor de QTextBrowser
    QTextBrowser* _kaartenbakDisplay = nullptr;
    std::unique_ptr<Drukbox> _drukbox;

    // Sloten per deur. d2 krijgt naast een gewone slot ook een
    // HerkenningsSlot; die houden we als typed pointer voor de
    // extra UI-acties (voegToegang, voegBlokkeer, toonKaartenbak).
    std::vector<std::shared_ptr<domain::Slot>> _slotenVd;
    std::vector<std::shared_ptr<domain::Slot>> _slotenD1;
    std::vector<std::shared_ptr<domain::Slot>> _slotenD2;
    std::shared_ptr<domain::HerkenningsSlot>   _herkenningD2;

    // Domain
    domain::HallSensor _halsensor;
    domain::Schuifdeur _vd;
    domain::Draaideur  _d1;
    domain::Draaideur  _d2;

    // UI widgets
    QLineEdit*   _inputVd   = nullptr;
    QLineEdit*   _inputD1   = nullptr;
    QLineEdit*   _inputD2   = nullptr;
    QLabel*      _statusVd  = nullptr;
    QLabel*      _statusD1  = nullptr;
    QLabel*      _statusD2  = nullptr;
};

} // namespace ui

#endif // SWADP_OOPR2_MAINWINDOW_H
