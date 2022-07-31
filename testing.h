#ifndef TESTING_H
#define TESTING_H

#include <QWidget>

#include <QSql>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlRecord>

#include <QFile>

#include <QProcess>
#include <QVector>

#include <QMessageBox>

#include <QDebug>

#include <QPainter>

#include "testitem.h"
#include "setings.h"

namespace Ui {
class testing;
}

class testing : public QWidget
{
    Q_OBJECT

signals:
    void notSetUser();

public slots:
    void setDataUser(QString, int);
    void slotSetTableTest();
    void setTests(QVector<Test*>*);

public:
    explicit testing(QWidget *parent = 0);
    void setSetings(Setings*);

    ~testing();

private slots:
    void slotTableTestClicked(int row, int col);
    void slotStarTest();
    void slotSetColorTable();
    void slotRepaintTableUser();

protected:
    void paintEvent();

private:
    Setings* setingsApp;
    bool setUser;

    void setStyle();

    int numRow;
    int numColumn;
    bool buildTable;

    QVector <Test*>* testTable;

    bool readyStartTest;
    int errorStartTest;

    QString dataUser;
    int idCadet;

    QString dataTest;
    QString allData;

    QString nameUserFile = "VavkoTest\\User.txt";
    QFile fileUserText;

    //QStringList namesTest;
    QStringList namesColumn;

    Ui::testing *ui;
};

#endif // TESTING_H
