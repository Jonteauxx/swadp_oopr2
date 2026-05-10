/**
 * @file main.cpp
 * @brief Entry-point van de GebouwBeheer-applicatie voor firma L&B.
 * @author tj.herdigein
 * @date 2026
 *
 * Voor Fase 0 toont dit alleen een leeg venster zodat we kunnen
 * verifieren dat de Qt-toolchain op de Raspberry Pi werkt. In Fase 3
 * wordt dit vervangen door MainWindow met de plattegrond en de deuren.
 */

#include <QApplication>
#include <QLabel>

/**
 * @brief Start de Qt-applicatie en toont een placeholder-venster.
 *
 * @param argc Aantal command-line argumenten (door Qt gebruikt).
 * @param argv Command-line argumenten (door Qt gebruikt).
 * @return Exit-code van de Qt-eventloop.
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QLabel label("L&B GebouwBeheer - skeleton (Fase 0)");
    label.setMinimumSize(400, 200);
    label.setAlignment(Qt::AlignCenter);
    label.show();

    return app.exec();
}
