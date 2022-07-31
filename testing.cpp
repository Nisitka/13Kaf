#include "testing.h"
#include "ui_testing.h"

#include "setcoloritem.h"
#include "winerror.h"

testing::testing(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::testing)
{
    ui->setupUi(this);

    this->setWindowTitle("Тестирование");

    numRow = 0;
    buildTable = false; //таблица еще не построена

    setUser = false;
    ui->nameUserLabel->setStyleSheet("color: rgb(240, 0, 0);");
    ui->nameUserLabel->setText("Пользователь не выбран!");
    ui->repaintButton->setEnabled(false);

    ui->StartTestButton->setEnabled(false);
    readyStartTest = false;

    connect(
            ui->exitButton, SIGNAL(clicked()),
            this, SLOT(hide())
            );
    connect(
            ui->testTableWidget, SIGNAL(cellClicked(int, int)),
            this, SLOT(slotTableTestClicked(int, int))
            );
    connect(
            ui->StartTestButton, SIGNAL(clicked()),
            this, SLOT(slotStarTest())
            );
    connect(
            ui->repaintButton, SIGNAL(clicked()),
            this, SLOT(slotRepaintTableUser())
            );

    fileUserText.setFileName(QCoreApplication::applicationDirPath() + "\\" + nameUserFile); //инициализация имени(пути) файла с данными об тесте и курсанте
    //если файла с инфой о курсанте не существует, то создать его
    if (!fileUserText.exists())
    {
        fileUserText.open(QIODevice::WriteOnly);
        fileUserText.close();
    }

    //slotSetTableTest();
    setStyle();
}

void testing::slotRepaintTableUser()
{
    ui->repaintButton->setEnabled(false);
    slotSetColorTable();
}

void testing::setTests(QVector<Test*>* Theme)
{
    testTable = Theme;
    qDebug() << "темы получены!!!";
}

void testing::setDataUser(QString dataUser_, int idCadet_)
{
    idCadet = idCadet_;
    dataUser = dataUser_;

    QSqlQuery query;
    query.exec("SELECT * FROM cadet WHERE id = '" + QString::number(idCadet) + "';");
    QSqlRecord rec = query.record();
    query.next();
    ui->nameUserLabel->setText(query.value(rec.indexOf("name")).toString());
    ui->nameUserLabel->setStyleSheet("color: rgb(0, 0, 0);");

    setUser = true;
    slotSetColorTable();

    readyStartTest = true;
}

void testing::setSetings(Setings* setingsApp_)
{
    setingsApp = setingsApp_;
}

void testing::slotStarTest()
{
    if (readyStartTest)
    {
        fileUserText.open(QIODevice::WriteOnly);

        QTextStream out0(&fileUserText);

        allData = dataUser + dataTest;
        out0 << allData; //"Иванов Иван Иванович2*220*ДС-131*Знать*2*";
        fileUserText.close();

        QString nametester = QCoreApplication::applicationDirPath() + "\\VavkoTest\\VavkoTest.exe";
        QProcess::startDetached(nametester);

        ui->repaintButton->setEnabled(true);
    }
    else
    {
        emit notSetUser();
        new winError("Пользователь не выбран!", "ОК");
    }
}

void testing::slotTableTestClicked(int row, int col)
{
    int idTestStart;
    QString competenc;

    switch (col) {
    case 0:
        ui->StartTestButton->setEnabled(false);
        break;
    case 1:
        if (testTable->at(row)->z)
        {
            ui->StartTestButton->setEnabled(true);
            idTestStart = testTable->at(row)->idTest_Z;
            competenc =  "Знать";
        }
        else
        {
            ui->StartTestButton->setEnabled(false);
        }
        break;
    case 2:
        if (testTable->at(row)->y)
        {
            ui->StartTestButton->setEnabled(true);
            idTestStart = testTable->at(row)->idTest_Y;
            competenc =  "Уметь";
        }
        else
        {
            ui->StartTestButton->setEnabled(false);
        }
        break;
    case 3:
        ui->StartTestButton->setEnabled(false);
        break;

    }

    dataTest = "ДС-1314*" +
                competenc + "*" +
                QString::number(idTestStart)+ "*";
}

void testing::slotSetTableTest()
{
    if (!buildTable)
    {
        testTable = setingsApp->getTests();
        numRow = testTable->size();

        namesColumn << "№ темы"
                    << "Знания"
                    << "Умения"
                    << "Тема теста";

        QStringList namesRow;
        for (int i=0; i<numRow; i++)
        {
            namesRow << "";
        }

        numColumn = namesColumn.size();

        ui->testTableWidget->setColumnCount(numColumn);
        ui->testTableWidget->setRowCount(numRow);

        ui->testTableWidget->setHorizontalHeaderLabels(namesColumn);
        ui->testTableWidget->setVerticalHeaderLabels(namesRow);

        //создание и заполнение таблицы
        QSqlQuery query;
        for (int i = 0; i<numRow; i++)
        {
            QVector <QTableWidgetItem*> Items;
            for (int k=0; k<numColumn; k++)
            {
                Items.append(new QTableWidgetItem);
            }

            query.exec("SELECT distinct num_theme FROM test WHERE name = '" + testTable->at(i)->name + "';");
            QSqlRecord rec1 = query.record();
            query.next();

            Items[0]->setText(query.value(rec1.indexOf("num_theme")).toString());

            if (testTable->at(i)->z)
            {
                Items[1]->setBackground(QColor(211, 236, 185, 255));
            }
            else
            {
                //Items[1]->setBackground(QColor(255, 255, 255, 255));
            }
            if (testTable->at(i)->y)
            {
                Items[2]->setBackground(QColor(211, 236, 185, 255));
            }
            else
            {
                //Items[2]->setBackground(QColor(255, 255, 255, 255));
            }
            Items[3]->setText(testTable->at(i)->name);

            for (int j=0; j<numColumn; j++)
            {
                ui->testTableWidget->setItem(i, j, Items[j]);
            }

        }

        ui->testTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->testTableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);  //колонку названия теста растянуть до конца

        //установка размера таблицы
        ui->testTableWidget->setMaximumHeight(ui->testTableWidget->rowHeight(0) * (numRow + 2) - 5);

        //установка размеров окна
        this->setMaximumHeight(
                               ui->testTableWidget->maximumHeight() +
                               ui->exitButton->size().height() +
                               25
                               );
        this->setMaximumWidth(
                              ui->testTableWidget->maximumWidth()
                              );
        ui->testTableWidget->setColumnWidth(0, 90);
        ui->testTableWidget->setColumnWidth(1, 90);
        ui->testTableWidget->setColumnWidth(2, 90);
        ui->testTableWidget->setColumnWidth(3, 700);

        buildTable = true;
    }
}

void testing::slotSetColorTable()
{
    if (setUser)
    {
        //возращение состояния таблицы в исходное состояние
        for (int i = 0; i<numRow; i++)
        {
            if (testTable->at(i)->z)
            {
                //ui->testTableWidget->item(i, 1)->setText("не пройден");
                ui->testTableWidget->item(i, 1)->setTextColor(QColor(190, 190, 190));
                ui->testTableWidget->item(i, 1)->setBackground(QColor(211, 236, 185, 255));
            }
            if (testTable->at(i)->y)
            {
                //ui->testTableWidget->item(i, 2)->setText("не пройден");
                ui->testTableWidget->item(i, 2)->setTextColor(QColor(190, 190, 190));
                ui->testTableWidget->item(i, 2)->setBackground(QColor(211, 236, 185, 255));
            }
        }

        QSqlQuery query;

        //                       подцветка полей (в зависимости от данных пользователя)
        //не забудь выбирать только по max-му id при одинаковых значениях

        //для начала закрасим все так, словно все не пройдено
        for (int i = 0; i<numRow; i++)
        {
            query.exec("SELECT distinct num_theme FROM test WHERE name = '" + testTable->at(i)->name + "';");
            QSqlRecord rec1 = query.record();
            query.next();

            ui->testTableWidget->item(i, 0)->setText(query.value(rec1.indexOf("num_theme")).toString());

            //если тесты такие есть
            if (testTable->at(i)->z)
            {
                ui->testTableWidget->item(i, 1)->setBackground(QColor(211, 236, 185, 255));
                ui->testTableWidget->item(i, 1)->setText("не пройдено");
            }
            if (testTable->at(i)->y)
            {
                ui->testTableWidget->item(i, 2)->setBackground(QColor(211, 236, 185, 255));
                ui->testTableWidget->item(i, 2)->setText("не пройдено");
            }
        }

        //находим все результаты выбранного нами курсанта
        query.exec("SELECT * FROM result WHERE cadet_id = '" + QString::number(idCadet) + "';");
        int idResult = 0;
        int idTest;
        while (query.next())
        {
            QSqlRecord rec = query.record();
            //id результата и теста конкретного курсанта
            idResult = query.value(rec.indexOf("id")).toInt();
            idTest = query.value(rec.indexOf("test_id")).toInt();

            //если результат есть то искать где отобразить его
            if (idResult != 0)
            {
                //по i понимаем какая строка в TableWidgets, по типу теста какая колонка
                //прогоняем по колонке "знать" и "уметь"
                int proc;
                for (int i=0; i<testTable->size(); i++)
                {
                    if (idTest == testTable->at(i)->idTest_Z)
                    {
                        proc = ((double)query.value(rec.indexOf("count_of_correct")).toInt() /
                                             query.value(rec.indexOf("count_of_wrong")).toInt()) * 100;
                        //подцветка в зависимости от процента
                        ui->testTableWidget->item(i, 1)->setTextColor(Qt::black);

                        //защита от неккоректных данных
                        if (proc > 100) proc = 100;
                        if (proc < 0) proc = 0;

                        ui->testTableWidget->item(i, 1)->setBackground(setColorTableItem(proc));
                        ui->testTableWidget->item(i, 1)->setText(QString::number(proc) + "%");
                        break;
                    }
                }
                //прогоняем по колонке "уметь"
                for (int i=0; i<testTable->size(); i++)
                {
                    if (idTest == testTable->at(i)->idTest_Y)
                    {
                        proc = ((double)query.value(rec.indexOf("count_of_correct")).toInt() /
                                        query.value(rec.indexOf("count_of_wrong")).toInt()) * 100;

                        //защита от неккоректных данных
                        if (proc > 100) proc = 100;
                        if (proc < 0) proc = 0;

                        //подцветка
                        ui->testTableWidget->item(i, 2)->setTextColor(Qt::black);
                        ui->testTableWidget->item(i, 2)->setBackground(setColorTableItem(proc));
                        ui->testTableWidget->item(i, 2)->setText(QString::number(proc) + "%");
                        break;
                    }
                }
            }
            idResult = 0;
        }
    }
    else
    {
        emit notSetUser();
    }

}

void testing::setStyle()
{
    ui->exitButton->setStyleSheet("QPushButton"
                                  "{"
                                  "     background: rgb(210, 47, 30);"
                                  "     border: 1px solid black;"
                                  "}"
                                  "QPushButton:hover"
                                  "{"
                                  "     background: rgb(255, 47, 0);"
                                  "     border: 1px solid black;"
                                  "}"
                                  );

    ui->StartTestButton->setStyleSheet("QPushButton"
                                  "{"
                                  "     background: rgb(128, 210, 53);"
                                  "     border: 1px solid black;"
                                  "}"
                                  "QPushButton:hover"
                                  "{"
                                  "     background: rgb(128, 255, 0);"
                                  "     border: 1px solid black;"
                                  "}"
                                  );

    ui->repaintButton->setStyleSheet("QPushButton"
                                  "{"
                                  "     border: 1px solid black;"
                                  "}"
                                  "QPushButton:hover"
                                  "{"
                                  "     background: rgb(255, 255, 255);"
                                  "     border: 1px solid black;"
                                  "}"
                                  );
}

void testing::paintEvent()
{
    //QPainter painter
}

testing::~testing()
{
    delete ui;
}
