#ifndef MATCHREZULTOBJECT_H
#define MATCHREZULTOBJECT_H

#include <QString>
#include <QVector>

#include "theme.h"

class matchRezultObject
{
public:
    matchRezultObject();

    QVector <rezTheme*>* getRezThemes();
    void addTheme(rezTheme* newTheme);

    void setTitle(QString);
    QString getTitle();

private:
    QString TitleRez;
    QVector <rezTheme*> rezThemes;

};

#endif // MATCHREZULTOBJECT_H
