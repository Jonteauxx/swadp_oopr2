/**
 * @file MainWindow.h
 * @brief Hoofdvenster van de GebouwBeheer-applicatie (opdracht 1 t/m 3).
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
#include <vector>

class QLabel;
class QLineEdit;
class QPushButton;

namespace ui {

/**
 * @class MainWindow
 * @brief Plattegrond + hardware + UI voor (ont)grendelen.
 *
 * Per opdracht 3 kan elke deur 0..n sloten hebben. We bewaren per deur
 * een std::vector<std::shared_ptr<Slot>> zodat we via één UI-input alle
 * sloten van die deur kunnen proberen te ontgrendelen.
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

    // Hardware-laag
    std::unique_ptr<infra::IGpio> _gpio;
    std::unique_ptr<infra::Servo> _servoVd;
    std::unique_ptr<infra::Servo> _servoD1;
    std::unique_ptr<infra::Servo> _servoD2;

    // Sloten - vectors voor opdracht 3 (meerdere per deur mogelijk).
    // Houden we ook hier los van Deur zodat de UI-handlers ze kunnen
    // bedienen zonder Deur opnieuw te bevragen.
    std::vector<std::shared_ptr<domain::Slot>> _slotenVd;
    std::vector<std::shared_ptr<domain::Slot>> _slotenD1;
    std::vector<std::shared_ptr<domain::Slot>> _slotenD2;

    // Domain
    domain::HallSensor _halsensor;
    domain::Schuifdeur _vd;
    domain::Draaideur  _d1;
    domain::Draaideur  _d2;

    // Slot-UI widgets
    QLineEdit*   _inputVd   = nullptr;
    QLineEdit*   _inputD1   = nullptr;
    QLineEdit*   _inputD2   = nullptr;
    QLabel*      _statusVd  = nullptr;
    QLabel*      _statusD1  = nullptr;
    QLabel*      _statusD2  = nullptr;
};

} // namespace ui

#endif // SWADP_OOPR2_MAINWINDOW_H
