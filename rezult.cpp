#include "rezult.h"
#include "ui_rezult.h"

#include <QDebug>

#include "winerror.h"

#include <QHeaderView>

Rezult::Rezult(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Rezult)
{
    ui->setupUi(this);

    this->setWindowTitle("Результаты");
    this->setFixedSize(1050, 730);
    setStyle(); //отрисовка дизайна

    setThemes = false;

    //выпадающий список выбора множества для оценки
    QStringList listTypeGroup;
    listTypeGroup << cadet << grupa << kurs;
    connect(
            ui->typeComboBox, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(setUnitsComboBox(QString))
            );
    ui->typeComboBox->addItems(listTypeGroup);

    connect(
            ui->kursCadetComboBox, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(setGroupsCadetCB(QString))
            );
    connect(
            ui->kursGroupComboBox, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(setGroupsGroupCB(QString))
            );
    connect(
            ui->groupCadetComboBox, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(setCadetsCadetCB())
            );
    connect(
            ui->buildRezultsButton, SIGNAL(clicked()),
            this, SLOT(matchRezul())
            );
    connect(
            ui->exitButton, SIGNAL(clicked()),
            this, SLOT(hide())
            );

    //типы результатов
    QStringList typeRezult;
    typeRezult << "по темам" << "по компетенциям";
    ui->typeRezultComboBox->addItems(typeRezult);

    table = new QTableWidget;
}

//установка настроек
void Rezult::setSetings(Setings* setingsApp_)
{
    setingsApp = setingsApp_;

    ui->typeComboBox->currentIndexChanged(cadet); //установка изначально типа "курсант"

    //если к бд подключены
    if (setingsApp->getConnectDB())
    {
        startDB();
    }
    else
    {
        ui->infoBDLabel->setText("База данных не подключена!");
        ui->infoBDLabel->setStyleSheet("color: rgb(240, 0, 0);");
    }

}

void Rezult::showEvent(QShowEvent* e)
{
    //if (!setingsApp->getConnectDB()) winError* error = new winError("База данных не подключена!", "Понятно");
}

void Rezult::startDB()
{
    ui->infoBDLabel->clear();
    setKursCB();
    Tests = setingsApp->getTests();
    buildThemes();
    setThemes = true;
    matchRezul(); //сразуже расчитать курсанта чтобы не было пусто
}

void Rezult::setKursCB()
{
    QSqlQuery query;
    query.exec("SELECT * FROM kurs;");
    QSqlRecord rec1 = query.record();
    QStringList numNaborList;
    while(query.next())
    {
        numNaborList << query.value(rec1.indexOf("number")).toString();
    }
    ui->kursCadetComboBox->addItems(numNaborList);
    ui->kursGroupComboBox->addItems(numNaborList);
    ui->kursKursComboBox->addItems(numNaborList);
}

//узнаем какие темы и какие в них тесты(пары знать и уметь)
void Rezult::buildThemes()
{
    QSqlQuery query;
    query.exec("SELECT * FROM theme;");
    QSqlRecord rec = query.record();

    //установка кол-ва тем и их имен
    while(query.next())
    {
        Themes.append(new Theme);
        Themes.last()->name = query.value(rec.indexOf("name")).toString();
        Themes.last()->id = query.value(rec.indexOf("id")).toInt();
    }
    numThemes = Themes.size();

    //добавление тестов в темы
    for (int i=0; i<numThemes; i++)
    {
        for (int j=0; j<Tests->size(); j++)
        {
            if (Themes.at(i)->id == Tests->at(j)->idTheme)
            {
                Themes.at(i)->Tests.append(Tests->at(j));
            }
        }
    }
}

void Rezult::setUnitsComboBox(QString type)
{
    QSqlQuery query;
    if (type == cadet)
    {
        ui->kursCadetComboBox->currentIndexChanged(ui->kursCadetComboBox->currentText());

        ui->stackedWidget->setCurrentIndex(0);
    }
    else
    {
        if (type == grupa)
        {
            ui->kursGroupComboBox->currentIndexChanged(ui->kursGroupComboBox->currentText());

            ui->stackedWidget->setCurrentIndex(1);
        }
        else
        {
            ui->stackedWidget->setCurrentIndex(2);
        }
    }
}

void Rezult::setGroupsGroupCB(QString numberKurs)
{
    QSqlQuery query;
    query.exec("SELECT * FROM groups WHERE kurs = '" + numberKurs + "';");
    QSqlRecord rec = query.record();
    QStringList groups;
    while(query.next())
    {
        groups << query.value(rec.indexOf("number")).toString();
    }
    ui->groupGroupComboBox->clear();
    ui->groupGroupComboBox->addItems(groups);
}

//установка знач. выпадающего списка групп в разделе курсант
void Rezult::setGroupsCadetCB(QString numberKurs)
{
    QSqlQuery query;
    query.exec("SELECT * FROM groups WHERE kurs = '" + numberKurs + "';");
    QSqlRecord rec = query.record();
    QStringList groups;
    while(query.next())
    {
        groups << query.value(rec.indexOf("number")).toString();
    }
    ui->groupCadetComboBox->clear();
    ui->groupCadetComboBox->addItems(groups);
}

//установка знач. выпадающего списка курсантов в разделе курсант
void Rezult::setCadetsCadetCB()
{
    QString numberGroup = ui->groupCadetComboBox->currentText();
    QString numberNabor = ui->kursCadetComboBox->currentText();

    //id группы
    QStringList cadets;
    int idGroup = getIDgroup(numberGroup, numberNabor);
    cadets = getNamesCadetGroup(idGroup);

    ui->fioCadetComboBox->clear();
    ui->fioCadetComboBox->addItems(cadets);
}

void Rezult::setRezult()
{
    setGraphicRezult();
    setTableRezult();
}

void Rezult::setTableRezult()
{
    ui->tableVerticalLayout->removeWidget(table);
    table = new QTableWidget;

    QStringList namesColumn;

    switch (typeRezult)
    {
    case 0:
        namesColumn << "№ Темы"
                    << "Название"
                    << "Знать, %"
                    << "Уметь, %"
                    << "Общее, %";
        break;
    case 1:
        namesColumn << "Компетенция"
                    << "ИД-1, %"
                    << "ИД-2, %"
                    << "Общее, %";
        break;
    }
    int numColumnTable = namesColumn.size();

    QStringList namesRow;
    for (int i=0; i<numThemes; i++)
    {
        namesRow << "";
    }

    table->setColumnCount(numColumnTable);
    int numRow;
    switch (typeRezult)
    {
    case 0:
        numRow = numThemes;
        break;
    case 1:
        numRow = numComp;
        break;
    }
    table->setRowCount(numRow);

    table->setHorizontalHeaderLabels(namesColumn);
    table->setVerticalHeaderLabels(namesRow);

    qDebug() << numRow << typeRezult << numColumnTable;
    //заполнение таблицы
    for (int i=0; i<numRow; i++)
    {
        QVector <QTableWidgetItem*> Items;
        for (int k=0; k<numColumnTable; k++)
        {
            Items.append(new QTableWidgetItem);
        }

        switch (typeRezult)
        {
        case 0:
            Items[0]->setText(QString::number(i + 1));
            Items[1]->setText(nameThemes.at(i));
            Items[2]->setText(QString::number(rez->getRezThemes()->at(i)->rezZ));
            Items[3]->setText(QString::number(rez->getRezThemes()->at(i)->rezY));
            Items[4]->setText(QString::number(
                                              (rez->getRezThemes()->at(i)->rezZ + rez->getRezThemes()->at(i)->rezY) / 2
                                              ));
            break;
        case 1:
            int sumRezZ = 0;
            int sumRezY = 0;

            //ОПК8 или ОПК5
            if (i == 0)
            {
                for (int i=0; i<6; i++)
                {
                    sumRezZ += rez->getRezThemes()->at(i)->rezZ;
                    sumRezY += rez->getRezThemes()->at(i)->rezY;
                }
                sumRezZ = sumRezZ / 6;
                sumRezY = sumRezY / 6;
            }
            else
            {
                for (int i=6; i<9; i++)
                {
                    sumRezZ += rez->getRezThemes()->at(i)->rezZ;
                    sumRezY += rez->getRezThemes()->at(i)->rezY;
                }
                sumRezZ = sumRezZ / 3;
                sumRezY = sumRezY / 3;
            }

            Items[0]->setText(nameComp.at(i));
            Items[1]->setText(QString::number(sumRezZ));
            Items[2]->setText(QString::number(sumRezY));
            Items[3]->setText(QString::number(
                                              (sumRezZ + sumRezY) / 2
                                              )
                              );
            break;
        }

        for (int j=0; j<numColumnTable; j++)
        {
            table->setItem(i, j, Items[j]);
        }
    }

    int z = 0;
    int y = 0;
    for (int i=0; i<numRow; i++)
    {
        switch (typeRezult)
        {
        case 0:
            z += table->item(i, 2)->text().toInt();
            y += table->item(i, 3)->text().toInt();
            break;
        case 1:
            z += table->item(i, 1)->text().toInt();
            y += table->item(i, 2)->text().toInt();
            break;
        }
    }

    switch (typeRezult)
    {
    case 0:
        ui->rezulZLabel->setText("Усвоение теории (знать) - " + QString::number(z/numRow) + "%");
        ui->rezulYLabel->setText("Усвоение практики (уметь) - " + QString::number(y/numRow) + "%");
        ui->rezulDiscipLabel->setText("Усвоение дисциплины - " + QString::number(((z+y)/(numRow*2))) + "%");
        break;
        break;
    case 1:
        ui->rezulZLabel->setText("Усвоение клмпетенции ИД1 - " + QString::number(z/numRow) + "%");
        ui->rezulYLabel->setText("Усвоение клмпетенции ИД2 - " + QString::number(y/numRow) + "%");
        ui->rezulDiscipLabel->setText("Усвоение дисциплины - " + QString::number(((z+y)/(numRow*2))) + "%");
        break;
    }

    //настройка размеров
    switch (typeRezult)
    {
    case 0:
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
        break;
    case 1:
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
        break;
    }

    ui->tableVerticalLayout->addWidget(table);
}

void Rezult::setGraphicRezult()
{
    //typeRezult = ui->typeRezultComboBox->currentIndex();
    switch (typeRezult)
    {
    case 0:
        numColumnGraphic = numThemes;
        //добавления имен тем
        nameThemes.clear();
        for (int i=0; i<numThemes; i++)
        {
             nameThemes.append(rez->getRezThemes()->at(i)->name);
        }
        break;
    case 1:
        numComp = nameComp.size();
        numColumnGraphic = numComp;
        break;
    }

    ui->graphicVerticalLayout->removeWidget(chartView);

    //цветовая гамма
    series = new QBarSeries();
    switch (typeRezult)
    {
    case 0:
        ZBarSet = new QBarSet("Знать");
        YBarSet = new QBarSet("Уметь");
        break;
    case 1:
        ZBarSet = new QBarSet("ИД-1");
        YBarSet = new QBarSet("ИД-2");
        break;
    }

    ZBarSet->setColor(QColor(112, 202, 22, 255));
    YBarSet->setColor(QColor(232, 164, 23, 255));

    switch (typeRezult)
    {
    case 0:
        for (int i = 0; i<numColumnGraphic; i++)
        {
            *ZBarSet << rez->getRezThemes()->at(i)->rezZ;
            *YBarSet << rez->getRezThemes()->at(i)->rezY;
        }
        break;
    case 1:
        int sumRezZ = 0;
        int sumRezY = 0;
        for (int i=0; i<6; i++)
        {
            sumRezZ += rez->getRezThemes()->at(i)->rezZ;
            sumRezY += rez->getRezThemes()->at(i)->rezY;
        }
        *ZBarSet << sumRezZ / 6;
        *YBarSet << sumRezY / 6;

        sumRezZ = 0;
        sumRezY = 0;
        for (int i=6; i<9; i++)
        {
            sumRezZ += rez->getRezThemes()->at(i)->rezZ;
            sumRezY += rez->getRezThemes()->at(i)->rezY;
        }
        *ZBarSet << sumRezZ / 3;
        *YBarSet << sumRezY / 3;
        break;
    }

    series->append(ZBarSet);
    series->append(YBarSet);

    chart = new QChart();
    chart->addSeries(series);

    //установка заголовка
    QString title = rez->getTitle();

    chart->setTitle(title);
    chart->setAnimationOptions(QChart::SeriesAnimations);

    axisX = new QBarCategoryAxis();

    //названия столбцов
    QStringList nameColumn;

    for (int i=0; i<numColumnGraphic; i++)
    {
        switch (typeRezult)
        {
        case 0:
            nameColumn.append("Тема №" + QString::number(i+1));
            break;
        case 1:
            nameColumn.append(nameComp.at(i));
            break;
        }
    }

    axisX->append(nameColumn);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    axisY = new QValueAxis();
    axisY->setRange(0, 100);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    chartView = new QChartView(chart);
    chartView->setParent(this);
    chartView->setRenderHint(QPainter::Antialiasing);

    ui->graphicVerticalLayout->addWidget(chartView);
}

void Rezult::matchRezul()
{
    typeRezult = ui->typeRezultComboBox->currentIndex(); //тип расчетов

    switch (ui->stackedWidget->currentIndex()) {
    //расчет результатов курсанта
    case 0:
        rez = matchRezulCadet(ui->fioCadetComboBox->currentText());//теперь в результатах новый объект
        break;
    //расчет результатов группы
    case 1:
        rez = matchRezulGroup(ui->kursGroupComboBox->currentText(), ui->groupGroupComboBox->currentText());
        break;
    //расчет результатов курса(набора)
    case 2:
        rez = matchRezulNabor(ui->kursKursComboBox->currentText());
        break;
    }
    setRezult(); //отображаем результаты
    delete rez;  //после отображения очищаем память
    ui->rezulTabWidget->setCurrentIndex(0); //сразу же показать результаты
}

//расчет результатов одного курсанта
matchRezultObject* Rezult::matchRezulCadet(QString nameCadet)
{
    matchRezultObject* newRez = new matchRezultObject; //объект результата, на основе него строится диаграмма

    //поиск id выбранного курсанта
    QSqlQuery query;
    int idCadet;
    query.exec("SELECT * FROM cadet WHERE name = '" + nameCadet + "';");
    query.next();
    QSqlRecord rec = query.record();
    idCadet = query.value(rec.indexOf("id")).toInt();

    for (int i=0; i<Themes.size(); i++)
    {
        newRez->addTheme(new rezTheme);
        newRez->getRezThemes()->last()->name = Themes.at(i)->name;

        //подсчет среднего за "Знать"
        newRez->getRezThemes()->last()->rezZ = 0;
        for (int j=0; j<Themes.at(i)->Tests.size(); j++) //пробегаемся по всем тестам темы
        {
            //если тест на компетенцию "Знать" есть, то
            if (Themes.at(i)->Tests.at(j)->z)
            {
                //сделать с учетом того, что результатов одного теста может быть > 1
                query.exec("SELECT * FROM result WHERE cadet_id = '" + QString::number(idCadet) +
                           "' AND test_id = '" + QString::number(Themes.at(i)->Tests.at(j)->idTest_Z) + "';");
                QSqlRecord rec1 = query.record();

                if (query.next())
                {
                    newRez->getRezThemes()->last()->rezZ += ((double)query.value(rec1.indexOf("count_of_correct")).toInt() /
                                                        query.value(rec1.indexOf("count_of_wrong")).toInt()) * 100;
                }
            }
        }
        newRez->getRezThemes()->last()->rezZ =  newRez->getRezThemes()->last()->rezZ / Themes.at(i)->Tests.size();

        //подсчет среднего за "Уметь"
        newRez->getRezThemes()->last()->rezY = 0;
        for (int j=0; j<Themes.at(i)->Tests.size(); j++) //пробегаемся по всем тестам темы
        {
            //если тест на компетенцию "Знать" есть, то
            if (Themes.at(i)->Tests.at(j)->y)
            {
                //сделать с учетом того, что результатов одного теста может быть > 1
                query.exec("SELECT * FROM result WHERE cadet_id = '" + QString::number(idCadet) +
                           "' AND test_id = '" + QString::number(Themes.at(i)->Tests.at(j)->idTest_Y) + "';");
                QSqlRecord rec1 = query.record();

                if (query.next())
                {
                    newRez->getRezThemes()->last()->rezY += ((double)query.value(rec1.indexOf("count_of_correct")).toInt() /
                                                        query.value(rec1.indexOf("count_of_wrong")).toInt()) * 100;
                }
            }
        }
        newRez->getRezThemes()->last()->rezY = newRez->getRezThemes()->last()->rezY / Themes.at(i)->Tests.size();
    }

    switch (typeRezult)
    {
    case 0:
        newRez->setTitle("Тематическая оценка"
                         " результатов изучения "
                         "дисциплины курсанта " +
                         ui->fioCadetComboBox->currentText());
        break;
    case 1:
        newRez->setTitle("Компетентностная оценка "
                         "результатов изучения "
                         "дисциплины курсанта " +
                         ui->fioCadetComboBox->currentText());
        break;
    }
    //newRez->setTitle(ui->groupCadetComboBox->currentText() + " уч. гр. курсант " + ui->fioCadetComboBox->currentText());

    return newRez;
}

matchRezultObject* Rezult::matchRezulGroup(QString numNabor, QString numGroup)
{
    matchRezultObject* newRez = new matchRezultObject; //объект результата, на основе него строится диаграмма

    //поиск id выбранной группы
    int idGroup = getIDgroup(numGroup, numNabor);

    //получение имен курсантов
    QStringList namesCadet = getNamesCadetGroup(idGroup);

    newRez = matchRezulCadet(namesCadet.at(0));  //считается что одинаковых имен нету
    int numCadet = namesCadet.size();
    int numThemes = newRez->getRezThemes()->size();
    for (int i=1; i<numCadet; i++)
    {
        matchRezultObject* r = matchRezulCadet(namesCadet.at(i)); //получаем результаты очередного курсанта      
        for (int j=0; j<numThemes; j++)
        {
            newRez->getRezThemes()->at(j)->rezY += r->getRezThemes()->at(j)->rezY;
            newRez->getRezThemes()->at(j)->rezZ += r->getRezThemes()->at(j)->rezZ;
        }
    }

    //усреднение значений
    for (int i=0; i<numThemes; i++)
    {
        newRez->getRezThemes()->at(i)->rezY = (double) newRez->getRezThemes()->at(i)->rezY / numCadet;
        newRez->getRezThemes()->at(i)->rezZ = (double) newRez->getRezThemes()->at(i)->rezZ / numCadet;
    }

    switch (typeRezult)
    {
    case 0:
        newRez->setTitle("Тематическая оценка"
                         " результатов изучения "
                         "дисциплины " +
                         numGroup + "уч. группы, набор " + numNabor + "-го года");
        break;
    case 1:
        newRez->setTitle("Компетентностная оценка "
                         "результатов изучения "
                         "дисциплины " +
                         numGroup + "уч. группы, набор " + numNabor + "-го года");
        break;
    }

    return newRez;
}

matchRezultObject* Rezult::matchRezulNabor(QString numNabor)
{
    matchRezultObject* newRez = new matchRezultObject; //объект результата, на основе него строится диаграмма

    //получение номеров групп набора
    QStringList numberGroup;
    QSqlQuery query;
    query.exec("SELECT * FROM groups WHERE kurs = '" + numNabor + "';");
    QSqlRecord rec = query.record();
    while(query.next())
    {
        //qDebug() << query.value(rec.indexOf("number")).toString();
        numberGroup << query.value(rec.indexOf("number")).toString();
    }

    newRez = matchRezulGroup(numNabor, numberGroup.at(0));  //считается что одинаковых имен нету
    int numGroup = numberGroup.size();
    int numThemes = newRez->getRezThemes()->size();
    for (int i=1; i<numGroup; i++)
    {
        matchRezultObject* r = matchRezulGroup(numNabor, numberGroup.at(i)); //получаем результаты очередного курсанта
        for (int j=0; j<numThemes; j++)
        {
            newRez->getRezThemes()->at(j)->rezY += r->getRezThemes()->at(j)->rezY;
            newRez->getRezThemes()->at(j)->rezZ += r->getRezThemes()->at(j)->rezZ;
        }
    }

    //усреднение значений
    for (int i=0; i<numThemes; i++)
    {
        newRez->getRezThemes()->at(i)->rezY = (double) newRez->getRezThemes()->at(i)->rezY / numGroup;
        newRez->getRezThemes()->at(i)->rezZ = (double) newRez->getRezThemes()->at(i)->rezZ / numGroup;
    }

    switch (typeRezult)
    {
    case 0:
        newRez->setTitle("Тематическая оценка"
                         " результатов изучения "
                         "дисциплины набора " + numNabor + "-го года");
        break;
    case 1:
        newRez->setTitle("Компетентностная оценка "
                         "результатов изучения "
                         "дисциплины набора " + numNabor + "-го года");
        break;
    }

    return newRez;
}

int Rezult::getIDgroup(QString numGroup, QString numNabor)
{
    QSqlQuery query;
    query.exec("SELECT * FROM groups WHERE number = '" + numGroup + "' AND kurs = '" + numNabor + "';");
    query.next();
    QSqlRecord rec = query.record();
    return query.value(rec.indexOf("id")).toInt();
}

QString Rezult::getNumGroup(int idGroup)
{
    QSqlQuery query;
    query.exec("SELECT * FROM groups WHERE id = '" + QString::number(idGroup) + "';");
    query.next();
    QSqlRecord rec = query.record();
    return query.value(rec.indexOf("number")).toString();
}

QStringList Rezult::getNamesCadetGroup(int idGroup)
{
    QStringList cadets;

    QSqlQuery query;
    query.exec("SELECT * FROM cadet WHERE id_group = '" + QString::number(idGroup) + "';");
    QSqlRecord rec = query.record();
    while(query.next())
    {
        cadets << query.value(rec.indexOf("name")).toString();
    }

    return cadets;
}

void Rezult::setStyle()
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

    ui->buildRezultsButton->setStyleSheet("QPushButton"
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
}

Rezult::~Rezult()
{
    delete ui;
}
