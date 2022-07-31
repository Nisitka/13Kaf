#ifndef SETUSERFORM_H
#define SETUSERFORM_H

#include <QWidget>
#include <QPixmap>

#include <QSql>
#include <QSqlQuery>
#include <QSqlRecord>

#include "setings.h"

namespace Ui {
class setUserForm;
}

class setUserForm : public QWidget
{
    Q_OBJECT

signals:
    void setUser(QString nameUser, int numGroup);

public:
    explicit setUserForm(QWidget *parent = 0);

    void setSettings(Setings*);

    ~setUserForm();

private slots:
    void setKursComboBox();
    void setGroupConmoBox(QString numberKurs);
    void setCadetComboBox();

    void disableSetUser();

    void upCadet(); //поиск курсанта

    void setUser(); //подтвердить курсанта

private:
    void setDisene();
    QString nameResurses;

    Setings* settingsApp;

    Ui::setUserForm *ui;
};

#endif // SETUSERFORM_H
