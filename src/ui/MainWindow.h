/**
 * @file MainWindow.h
 * @brief Hoofdvenster van de GebouwBeheer-applicatie (opdracht 1).
 * @author tj.herdigein
 * @date 2026
 */

#ifndef SWADP_OOPR2_MAINWINDOW_H
#define SWADP_OOPR2_MAINWINDOW_H

#include "domain/Draaideur.h"
#include "domain/HallSensor.h"
#include "domain/Schuifdeur.h"

#include <QPixmap>
#include <QWidget>

namespace ui {

/**
 * @class MainWindow
 * @brief Tekent de plattegrond + 3 deuren + halsensor, en biedt 4 knoppen.
 *
 * Layout:
 *   - Links (20, 20):  Gebouw.png als achtergrond.
 *   - Op de PNG:        de 3 deuren en de halsensor (zelf-tekenend).
 *   - Rechts naast:     4 QPushButtons om elk element te bedienen.
 *
 * Architectuur-noot: dit is de UI-laag. In een latere fase introduceren
 * we een GebouwController die de domain-objecten beheert en de hardware
 * synchroniseert; voor opdracht 1 houdt MainWindow ze nog direct als
 * members, zoals in het docent-UML.
 */
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

protected:
    /// @brief Tekent achtergrond + alle domain-objecten.
    void paintEvent(QPaintEvent* event) override;

    /// @brief Debug: logt klik-coords naar stderr voor coordinaat-tuning.
    void mousePressEvent(QMouseEvent* event) override;

private slots:
    void onSchuifdeurKnopClicked();
    void onDraaideurD1KnopClicked();
    void onDraaideurD2KnopClicked();
    void onHalsensorKnopClicked();

private:
    /// Toggle hulpfunctie: open <-> dicht of activeer <-> deactiveer.
    void toggleEnHerteken();

    QPixmap _gebouw;

    // Volgorde-belangrijk: _halsensor moet vóór _vd staan want _vd
    // krijgt een pointer naar _halsensor in zijn constructor.
    domain::HallSensor _halsensor;
    domain::Schuifdeur _vd;
    domain::Draaideur  _d1;
    domain::Draaideur  _d2;
};

} // namespace ui

#endif // SWADP_OOPR2_MAINWINDOW_H
