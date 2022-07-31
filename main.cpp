#include <QApplication>

#include "app.h"

#include <QProgressBar>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationDisplayName("D://Belousov//build-Obolochka13Kaf-Desktop_Qt_5_12_9_MinGW_64_bit-Release//release//resursImage//loading.jpg");

    app Obolochka;
    Obolochka.start();

    return a.exec();
}
