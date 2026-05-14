/**
 * @file MainWindow.h
 * @brief Hoofdvenster van de GebouwBeheer-applicatie (opdracht 1 + hardware).
 * @author tj.herdigein
 * @date 2026
 */

#ifndef SWADP_OOPR2_MAINWINDOW_H
#define SWADP_OOPR2_MAINWINDOW_H

#include "domain/Draaideur.h"
#include "domain/HallSensor.h"
#include "domain/Schuifdeur.h"

#include "infra/IGpio.h"
#include "infra/Servo.h"

#include <QPixmap>
#include <QWidget>

#include <memory>

namespace ui {

/**
 * @class MainWindow
 * @brief Tekent de plattegrond + 3 deuren + halsensor, en bestuurt de
 *        fysieke servo's via een IGpio-facade.
 *
 * Architectuur:
 *   - _gpio is een unique_ptr<IGpio> die bij opstart probeert een
 *     PigpioGpio te maken; bij falen valt hij terug op MockGpio zodat
 *     de app ook zonder pigpiod start (handig voor visueel testen).
 *   - Servo's krijgen een referentie naar *_gpio.
 *   - Member-volgorde is belangrijk: _gpio moet bestaan vóór de Servo's
 *     die er een referentie aan houden (C++-init-volgorde).
 */
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

protected:
    /// @brief Tekent achtergrond + alle domain-objecten.
    void paintEvent(QPaintEvent* event) override;

private slots:
    void onSchuifdeurKnopClicked();
    void onDraaideurD1KnopClicked();
    void onDraaideurD2KnopClicked();
    void onHalsensorKnopClicked();

private:
    QPixmap _gebouw;

    // Hardware-laag: gpio moet vóór de servo's gedeclareerd staan zodat
    // hij eerst geinitialiseerd wordt.
    std::unique_ptr<infra::IGpio> _gpio;
    std::unique_ptr<infra::Servo> _servoVd;
    std::unique_ptr<infra::Servo> _servoD1;
    std::unique_ptr<infra::Servo> _servoD2;

    // Domain: _halsensor moet vóór _vd staan want _vd krijgt een pointer
    // naar _halsensor in zijn constructor.
    domain::HallSensor _halsensor;
    domain::Schuifdeur _vd;
    domain::Draaideur  _d1;
    domain::Draaideur  _d2;
};

} // namespace ui

#endif // SWADP_OOPR2_MAINWINDOW_H
