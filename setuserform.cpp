#include "setuserform.h"
#include "ui_setuserform.h"

#include "rezult.h"

setUserForm::setUserForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::setUserForm)
{
    ui->setupUi(this);

    this->setWindowTitle("Выбор пользователя");

    connect(
            ui->exitButton, SIGNAL(clicked()),
            this, SLOT(hide())
            );
    connect(
            ui->kursComboBox, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(setGroupConmoBox(QString))
            );
    connect(
            ui->groupComboBox, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(setCadetComboBox())
            );
    connect(
            ui->setUserButton, SIGNAL(clicked()),
            this, SLOT(setUser())
            );
    connect(
            ui->cadetComboBox, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(disableSetUser())
            );
    setDisene();
}

void setUserForm::setSettings(Setings* settingsApp_)
{
    settingsApp = settingsApp_;

    if (settingsApp->getConnectDB())
    {
        setKursComboBox();
    }
}

//поиск курсанта
void setUserForm::upCadet()
{

}

void setUserForm::setUser()
{
    QString dataUser = ui->cadetComboBox->currentText() + "*" +
                       ui->groupComboBox->currentText() + "*";

    //поиск id курсанта
    QSqlQuery query;
    query.exec("SELECT * FROM cadet WHERE name = '" +
               ui->cadetComboBox->currentText() + "';");
    QSqlRecord rec = query.record();
    int idCadet;
    query.next();
    idCadet = query.value(rec.indexOf("id")).toInt();

    emit setUser(dataUser, idCadet);

    ui->setUserButton->setEnabled(false);
    this->hide();
}

void setUserForm::setKursComboBox()
{
    //устанавливаем заранее список наборов
    QSqlQuery query;
    query.exec("SELECT * FROM kurs;");
    QSqlRecord rec1 = query.record();
    QStringList numNaborList;
    while(query.next())
    {
        numNaborList << query.value(rec1.indexOf("number")).toString();
    }
    ui->kursComboBox->addItems(numNaborList);

    disableSetUser();
}

void setUserForm::setGroupConmoBox(QString numberKurs)
{
    QSqlQuery query;
    query.exec("SELECT * FROM groups WHERE kurs = '" + numberKurs + "';");
    QSqlRecord rec = query.record();
    QStringList groups;
    while(query.next())
    {
        groups << query.value(rec.indexOf("number")).toString();
    }
    ui->groupComboBox->clear();
    ui->groupComboBox->addItems(groups);

    disableSetUser();
}

void setUserForm::setCadetComboBox()
{
    QString numberGroup = ui->groupComboBox->currentText();
    QString numberNabor = ui->kursComboBox->currentText();

    //id группы
    QStringList cadets;
    int idGroup = Rezult::getIDgroup(numberGroup, numberNabor);
    cadets = Rezult::getNamesCadetGroup(idGroup);

    ui->cadetComboBox->clear();
    ui->cadetComboBox->addItems(cadets);

    disableSetUser();
}

void setUserForm::disableSetUser()
{
    ui->setUserButton->setEnabled(true);
}

void setUserForm::setDisene()
{
    nameResurses = "resursImage";

    QPixmap* pixIcon;
    pixIcon = new QPixmap(
                       QCoreApplication::applicationDirPath() + "\\" +
                       nameResurses + "\\" +
                       "User.png"
                       );

    //кнопка закрытия
    ui->exitButton->setStyleSheet("QPushButton"
                                  "{"
                                  "     background: rgb(210, 47, 30);"
                                  "     border: 1px solid black;"
                                  "}"
                                  "QPushButton:hover"
                                  "{"
                                  "     background: rgb(255, 47, 0);"
                                  "     border: 1px solid rgb(100, 0, 15);"
                                  "}"
                                  );

    //кнопка применить
    ui->setUserButton->setStyleSheet("QPushButton"
                                  "{"
                                  "     background: rgb(128, 210, 53);"
                                  "     border: 1px solid black;"
                                  "}"
                                  "QPushButton:hover"
                                  "{"
                                  "     background: rgb(128, 255, 0);"
                                  "     border: 1px solid black;"
                                  "}"
                                  "QPushButton:pressed"
                                  "{"
                                  "     background: rgb(88, 170, 53);"
                                  "     border: 1px solid black;"
                                  "}"
                                  );
}

setUserForm::~setUserForm()
{
    delete ui;
}
