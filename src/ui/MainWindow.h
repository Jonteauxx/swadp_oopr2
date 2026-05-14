/**
 * @file MainWindow.h
 * @brief Hoofdvenster van de GebouwBeheer-applicatie (opdracht 1 + 2).
 * @author tj.herdigein
 * @date 2026
 */

#ifndef SWADP_OOPR2_MAINWINDOW_H
#define SWADP_OOPR2_MAINWINDOW_H

#include "domain/Draaideur.h"
#include "domain/HallSensor.h"
#include "domain/Schuifdeur.h"
#include "domain/Slot.h"

#include "infra/IGpio.h"
#include "infra/Servo.h"

#include <QPixmap>
#include <QWidget>

#include <memory>

class QLabel;
class QLineEdit;
class QPushButton;

namespace ui {

/**
 * @class MainWindow
 * @brief Tekent de plattegrond, bestuurt de fysieke servo's en biedt UI
 *        om sloten te (ont)grendelen (opdracht 2).
 *
 * Per deur biedt de UI:
 *   - een **deur-knop** (open/dicht toggle), die alleen werkt als het
 *     slot van die deur ontgrendeld is;
 *   - een **invoerveld** voor sleutel of code;
 *   - een **ontgrendel-knop** en een **vergrendel-knop**;
 *   - een **status-label** dat toont of het slot vergrendeld is.
 *
 * Per opdracht 2 vergrendelt het slot ook automatisch bij sluit().
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
    /// @brief Update alle status-labels op basis van de slot-staten.
    void updateSlotStatusLabels();

    QPixmap _gebouw;

    // Hardware-laag
    std::unique_ptr<infra::IGpio> _gpio;
    std::unique_ptr<infra::Servo> _servoVd;
    std::unique_ptr<infra::Servo> _servoD1;
    std::unique_ptr<infra::Servo> _servoD2;

    // Sloten (per opdracht 2)
    std::shared_ptr<domain::Slot> _slotVd;
    std::shared_ptr<domain::Slot> _slotD1;
    std::shared_ptr<domain::Slot> _slotD2;

    // Domain - _halsensor moet vóór _vd komen (constructor-volgorde).
    domain::HallSensor _halsensor;
    domain::Schuifdeur _vd;
    domain::Draaideur  _d1;
    domain::Draaideur  _d2;

    // Slot-UI widgets (3 deuren x 4 widgets per deur)
    QLineEdit*   _inputVd   = nullptr;
    QLineEdit*   _inputD1   = nullptr;
    QLineEdit*   _inputD2   = nullptr;
    QLabel*      _statusVd  = nullptr;
    QLabel*      _statusD1  = nullptr;
    QLabel*      _statusD2  = nullptr;
};

} // namespace ui

#endif // SWADP_OOPR2_MAINWINDOW_H
