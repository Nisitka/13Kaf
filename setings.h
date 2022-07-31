#ifndef SETINGS_H
#define SETINGS_H

#include <QWidget>

#include <QSql>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlRecord>

#include <QFile>
#include <QFileDialog>
#include <QTextStream>

#include <QMessageBox>

#include <QPainter>

#include "testitem.h"

namespace Ui {
class Setings;
}

class Setings : public QWidget
{
    Q_OBJECT

signals:
    void setSetingsSignal();

public:
    void paintEvent(QPaintEvent*);

    explicit Setings(QWidget *parent = 0);
    bool startDataBase();

    bool getConnectDB();
    QVector <Test*>* getTests();

    ~Setings();

private slots:
    void setDirDataBase();
    void setDataBase();
    void setSetings();

private:
    void setStyle();

    QString dirDataBase;
    QSqlDatabase db;
    bool connectDataBase;

    QFile fileDataBaseText;
    QString nameDBFile = "database.txt";

    QVector <Test*> Tests;
    void getTestsDataBase();

    QString nameResurses = "resursImage";

    int maxH = 400, maxW = 800;

    Ui::Setings *ui;
};

#endif // SETINGS_H
