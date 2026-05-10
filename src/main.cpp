/**
 * @file main.cpp
 * @brief Entry-point van de GebouwBeheer-applicatie voor firma L&B.
 * @author tj.herdigein
 * @date 2026
 *
 * Start de Qt-eventloop en toont het hoofdvenster met de plattegrond.
 */

#include "ui/MainWindow.h"

#include <QApplication>

/**
 * @brief Maakt de QApplication en toont MainWindow.
 *
 * @param argc Aantal command-line argumenten.
 * @param argv Command-line argumenten.
 * @return Exit-code van de Qt-eventloop.
 */
int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    ui::MainWindow venster;
    venster.show();

    return app.exec();
}
