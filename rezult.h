#ifndef REZULT_H
#define REZULT_H

#include <QWidget>
#include <QHBoxLayout>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

#include <QEvent>

#include <QSql>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlRecord>

#include <QTableWidgetItem>

#include "setings.h"
#include "matchrezultobject.h"
#include "testitem.h"

#include <QThread>

namespace Ui {
class Rezult;
}

QT_CHARTS_USE_NAMESPACE

class Rezult : public QWidget
{
    Q_OBJECT

signals:

public slots:
    void startDB();

public:
    explicit Rezult(QWidget *parent = 0);
    void setSetings(Setings*);

    static QString getNumGroup(int idGroup);
    static int getIDgroup(QString numGroup, QString numNabor);
    static QStringList getNamesCadetGroup(int idGroup);

    void showEvent(QShowEvent*);

    ~Rezult();

protected:

private slots:
    void setRezult();
    void setUnitsComboBox(QString);

    void setKursCB();
    void setGroupsGroupCB(QString numberKurs);
    void setGroupsCadetCB(QString numberKurs);
    void setCadetsCadetCB();

    void matchRezul();


private:
    Setings* setingsApp;

    void setGraphicRezult();
    QBarSet* ZBarSet;
    QBarSet* YBarSet;
    QStringList nameThemes;
    QStringList nameComp = {"ОПК 8",
                            "ОПК 5"};
    int numColumnGraphic;
    int typeRezult;
    QBarSeries* series;
    QChart* chart;
    QBarCategoryAxis* axisX;
    QValueAxis* axisY;
    QChartView* chartView;
    QTableWidget* table;

    void setTableRezult();

    int numThemes;
    int numComp;

    QString grupa = "группы",
            kurs = "набора",
            cadet = "курсанта";

    QVector <Test*>* Tests;
    struct Theme
    {
        //указатели на тесты конкретной темы
        QVector <Test*> Tests;
        int id;
        QString name; //название темы
    };
    QVector <Theme*> Themes;

    void buildThemes();

    matchRezultObject* rez;
    bool setThemes;

    matchRezultObject* matchRezulCadet(QString nameCadet);
    matchRezultObject* matchRezulGroup(QString numNabor, QString numGroup);
    matchRezultObject* matchRezulNabor(QString numNabor);

    void setStyle();

    Ui::Rezult *ui;
};

#endif // REZULT_H
