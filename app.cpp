#include "app.h"

app::app()
{
    winMenu = new menu;
    winSetings = new Setings;

    winMenu->setSetings(winSetings);

    winTesting = new testing;
    winTesting->setSetings(winSetings);

    winRezult = new Rezult;
    winRezult->setSetings(winSetings);

    winUsers = new setUserForm;
    winUsers->setSettings(winSetings);

    winLiter = new Literature;

    QObject::connect(
                     winMenu->getLinerButton(), SIGNAL(clicked()),
                     winLiter, SLOT(show())
                     );

    QObject::connect(
                     winMenu->getSetingsButton(), SIGNAL(clicked()),
                     winSetings, SLOT(show())
                     );
    QObject::connect(
                     winMenu, SIGNAL(openTesting()),
                     winTesting, SLOT(show())
                     );
    QObject::connect(
                     winMenu, SIGNAL(openUsers()),
                     winUsers, SLOT(show())
                     );
    QObject::connect(
                     winMenu, SIGNAL(openRezult()),
                     winRezult, SLOT(show())
                     );
    //отрисовка таблицы
    QObject::connect(
                     winMenu, SIGNAL(buildTableTest()),
                     winTesting, SLOT(slotSetTableTest())
                     );
    QObject::connect(
                     winUsers, SIGNAL(setUser(QString, int)),
                     winTesting, SLOT(slotSetTableTest())
                     );

    QObject::connect(
                     winUsers, SIGNAL(setUser(QString, int)),
                     winTesting, SLOT(setDataUser(QString, int))
                     );
    QObject::connect(
                     winTesting, SIGNAL(notSetUser()),
                     winUsers, SLOT(show())
                     );
    QObject::connect(
                     winSetings, SIGNAL(setSetingsSignal()),
                     winRezult, SLOT(startDB())
                     );
    QObject::connect(
                     winSetings, SIGNAL(setSetingsSignal()),
                     winUsers, SLOT(setKursComboBox())
                     );

    //закрытие приложения
    QObject::connect(
                     winMenu, SIGNAL(closeApp()),
                     winLiter, SLOT(close())
                     );
    QObject::connect(
                     winMenu, SIGNAL(closeApp()),
                     winSetings, SLOT(close())
                     );
    QObject::connect(
                     winMenu, SIGNAL(closeApp()),
                     winTesting, SLOT(close())
                     );
    QObject::connect(
                     winMenu, SIGNAL(closeApp()),
                     winRezult, SLOT(close())
                     );
    QObject::connect(
                     winMenu, SIGNAL(closeApp()),
                     winUsers, SLOT(close())
                     );
}

void app::start()
{
    winMenu->show();
}
