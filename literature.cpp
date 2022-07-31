#include "literature.h"
#include "ui_literature.h"

#include <QMessageBox>
#include <QHeaderView>

#include <QDebug>

Literature::Literature(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Literature)
{
    ui->setupUi(this);

    ui->tableWidgetBooks->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    connect(
            ui->exitButton, SIGNAL(clicked()),
            this, SLOT(hide())
            );

    //слот нажатия на элемент таблицы с файлами
    connect(
            ui->tableWidgetBooks, SIGNAL(itemClicked(QTableWidgetItem *)),
            this, SLOT(openhtmlfile(QTableWidgetItem *))
            );

    setBooks();

    setStyle();
}

void Literature::setStyle()
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

}

void Literature::setBooks()
{
    ui->tableWidgetBooks->clear();
    QString dirname_html = QCoreApplication::applicationDirPath()+ "/Books";
    this->setWindowTitle("Оглавление");

    QStringList list_html;
    QDir dir2(dirname_html);
    list_html = dir2.entryList(QStringList() << "*.mht");

    ui->tableWidgetBooks->setRowCount(list_html.size()); //кол-во строк
    ui->tableWidgetBooks->setColumnCount(1);

    QStringList nameColum("Главы учебника");
    ui->tableWidgetBooks->setHorizontalHeaderLabels(nameColum);

    for (int j = 0; j < list_html.size(); ++j)
    {
        QString name_html = list_html[j];
        QTableWidgetItem* newItem = new QTableWidgetItem(name_html.mid(0,name_html.size()-4));

        newItem->setTextAlignment(Qt::AlignVCenter);
        newItem->setData(Qt::UserRole, QVariant(dirname_html + "/" + name_html));
        newItem->setBackgroundColor(QColor(217, 242, 140));
        QString nameResurses = "resursImage";
        newItem->setBackground(QBrush(QPixmap(QCoreApplication::applicationDirPath() + "\\" +
                                              nameResurses + "\\" +
                                              "stringImage.jpg").scaled(this->size().width(), 30)));

        ui->tableWidgetBooks->setItem(j, 0, newItem);
    }
    ui->tableWidgetBooks->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
}

void Literature::openhtmlfile(QTableWidgetItem *item)
{
    QString namefile = item->data(Qt::UserRole).toString(); //dirname+"/"+item->text(); QString::fromLocal8Bit("D:/ГЛ 4.1.mht")
    QFile file1(namefile);
    if (file1.open(QIODevice::ReadOnly | QIODevice::Text))  //файл открыли
    {
        QDesktopServices::openUrl(QUrl(QUrl::fromLocalFile(namefile)));//QDesktopServices::openUrl(QUrl(QString::fromLocal8Bit("D:/ро.mht")));
    }
    else
        if (item->column() == 0) //выбран элемент нулевого столбца, т.е. с названием
            int n = QMessageBox::critical(0, "Внимание", "Документ находится в разработке или проблемы с открытием файла.", QMessageBox::Cancel);
    file1.close();
}

Literature::~Literature()
{
    delete ui;
}
