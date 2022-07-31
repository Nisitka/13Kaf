#ifndef APP_H
#define APP_H

#include <QObject>

//классы окон
#include "menu.h"
#include "literature.h"
#include "testing.h"
#include "rezult.h"
#include "setuserform.h"

//ядро приложения
class app
{
public:
    app();

    void start(); //запуск приложения

private:
    menu* winMenu; //окно меню
    Literature* winLiter;
    Setings* winSetings;
    testing* winTesting;
    Rezult* winRezult;
    setUserForm* winUsers;
};

#endif // APP_H
