#include "setings.h"
#include "ui_setings.h"

#include "winerror.h"
#include <QDebug>

Setings::Setings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Setings)
{
    ui->setupUi(this);

    this->setWindowTitle("Настройки");
    setMaximumHeight(maxH);

    setMaximumWidth(maxW);

    ui->pushButtonKonnnectBD->setEnabled(false);
    ui->setButton->setEnabled(false);

    connectDataBase = false;

    connect(ui->exitButton, SIGNAL(clicked()), this, SLOT(hide()));

    connect(ui->toolButtonDirBD, SIGNAL(clicked()), this, SLOT(setDirDataBase()));
    connect(ui->pushButtonKonnnectBD, SIGNAL(clicked()), this, SLOT(setDataBase()));

    //слот применения настроек
    connect(ui->setButton, SIGNAL(clicked()), this, SLOT(setSetings()));

    //если файла с данными базы данных не существует, то создать его
    fileDataBaseText.setFileName(QCoreApplication::applicationDirPath() + "\\" + nameDBFile);
    if (!fileDataBaseText.exists())
    {
        fileDataBaseText.open(QIODevice::WriteOnly);
        fileDataBaseText.close();
    }
    else
    {
        QString line;
        if (fileDataBaseText.open(QIODevice::ReadOnly))
        {
            QTextStream in(&fileDataBaseText);
            while (!in.atEnd())
                line = in.readLine();
            fileDataBaseText.close();
        }
        dirDataBase = line;
        ui->lineEditDirBD->setText(dirDataBase);
        setDataBase();
    }
    setStyle();
}

//получение данных об темах и передача данных об этом
void Setings::getTestsDataBase()
{
    QSqlQuery query;
    query.exec("SELECT distinct name FROM test;");
    QSqlRecord rec = query.record();

    //сбор названий тестов с базы данных
    while (query.next())
    {
        Tests.append(new Test);
        Tests.last()->name = query.value(rec.indexOf("name")).toString();
    }

    //сбор информации об id тестов, их компетенциях и id тем
    int idCompetence;
    int idTest;
    for (int i=0; i<Tests.size(); i++)
    {
        query.exec("SELECT * FROM test WHERE name = '" + Tests.at(i)->name + "';");
        QSqlRecord rec1 = query.record();

        while (query.next())
        {
            idCompetence = query.value(rec1.indexOf("competence_id")).toInt();
            idTest = query.value(rec1.indexOf("id")).toInt();
            //обращается для инфы о теме иногда 2 раза к бд, хотя достаточно на один тест 1-го раза
            Tests.at(i)->idTheme = query.value(rec1.indexOf("theme_id")).toInt();
            switch (idCompetence)
            {
            case 1:
                Tests.at(i)->z = true;
                Tests.at(i)->idTest_Z = idTest;
                break;
            case 2:
                Tests.at(i)->y = true;
                Tests.at(i)->idTest_Y = idTest;
                break;
            }
        }
    }
}

QVector <Test*>* Setings::getTests()
{
    return &Tests;
}

void Setings::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    QPixmap pix(
                QCoreApplication::applicationDirPath() + "\\" +
                nameResurses + "\\" +
                "fonClassic.jpg"
                );

    QRect rect(QPoint(0, 0), this->maximumSize());
    painter.drawPixmap(rect, pix);
}

void Setings::setStyle()
{
    ui->exitButton->setStyleSheet("QPushButton"
                                  "{"
                                  "     background: rgb(210, 47, 30);"
                                  "     border: 1px solid black;"
                                  "     border-radius: 5px;"
                                  "}"
                                  "QPushButton:hover"
                                  "{"
                                  "     background: rgb(255, 47, 0);"
                                  "     border: 1px solid black;"
                                  "     border-radius: 5px;"
                                  "}"
                                  );

    ui->setButton->setStyleSheet("QPushButton"
                                  "{"
                                  "     background: rgb(128, 210, 53);"
                                  "     border: 1px solid black;"
                                  "     border-radius: 5px;"
                                  "}"
                                  "QPushButton:hover"
                                  "{"
                                  "     background: rgb(128, 255, 0);"
                                  "     border: 1px solid black;"
                                  "     border-radius: 5px;"
                                  "}"
                                  );

    ui->pushButtonKonnnectBD->setStyleSheet("QPushButton"
                                       "{"
                                       "     background: rgb(240, 240, 240);"
                                       "     border: 1px solid black;"
                                       "}"
                                       "QPushButton:hover"
                                       "{"
                                       "     background: rgb(255, 255, 255);"
                                       "     border: 1px solid black;"
                                       "}"
                                       );

    ui->toolButtonDirBD->setStyleSheet("QPushButton"
                                       "{"
                                       "     background: rgb(240, 240, 240);"
                                       "     border: 1px solid black;"
                                       "}"
                                       "QPushButton:hover"
                                       "{"
                                       "     background: rgb(255, 255, 255);"
                                       "     border: 1px solid black;"
                                       "}"
                                       );
}

bool Setings::startDataBase()
{
    int sizeTextDir = dirDataBase.size();
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dirDataBase);

    //проверка расширения файла и его открытия
    if (dirDataBase.at(sizeTextDir - 2).toLatin1() + dirDataBase.at(sizeTextDir - 1) == "db" &
        db.open())
    {
        ui->label_startBD->setStyleSheet("color: rgb(0, 150, 0);");
        QString textConnect = "База данных подключена.";
        this->ui->label_startBD->setText(textConnect);

        ui->setButton->setEnabled(true);
        //при успешном поключении сохраняем путь к БД
        return true;
    }
    ui->label_startBD->setStyleSheet("color: rgb(240, 0, 0);");
    QString textConnect = "База данных не найдена.";
    this->ui->label_startBD->setText(textConnect);
    return false;
}

void Setings::setDirDataBase()
{
    dirDataBase = QFileDialog::getOpenFileName();

    //проверка пути к бд
    if (dirDataBase != NULL)
    {
        ui->lineEditDirBD->setText(dirDataBase);
        ui->pushButtonKonnnectBD->setEnabled(true);    //разблокирование кнопки подключения к БД
    }
    else
    {
        new winError("Фаил базы данных не выбран!", "ОК");
    }
}

void Setings::setDataBase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");

    ui->pushButtonKonnnectBD->setEnabled(false);

    if (dirDataBase != NULL)
    {
        db.setDatabaseName(dirDataBase);

        //при успешном подключении к БД установить значения по умолчанию
        if (startDataBase())
        {
            //Сохраним путь в файл хранения инфы о БД
            fileDataBaseText.remove();
            if (fileDataBaseText.open(QIODevice::WriteOnly))
            {
                QTextStream out(&fileDataBaseText);
                out << ui->lineEditDirBD->text() << "\n";
                fileDataBaseText.close();
            }
            getTestsDataBase(); //получение данных об темах и передача их
            connectDataBase = true;
        }
    }
    else
    {
        ui->label_startBD->setStyleSheet("color: rgb(240, 0, 0);");
        QString textConnect = "База данных не найдена.";
        this->ui->label_startBD->setText(textConnect);
    }
}

bool Setings::getConnectDB()
{
    return connectDataBase;
}

void Setings::setSetings()
{
    ui->setButton->setEnabled(false);

    emit setSetingsSignal();

    this->hide();
}

Setings::~Setings()
{
    delete ui;
}
