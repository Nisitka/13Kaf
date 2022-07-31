#include "matchrezultobject.h"

matchRezultObject::matchRezultObject()
{
    TitleRez = "";
}

QVector <rezTheme*>* matchRezultObject::getRezThemes()
{
    return &rezThemes;
}

void matchRezultObject::setTitle(QString title)
{
    TitleRez = title;
}

QString matchRezultObject::getTitle()
{
    return TitleRez;
}

void matchRezultObject::addTheme(rezTheme* newTheme)
{
    rezThemes.append(newTheme);
}
