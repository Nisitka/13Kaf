#ifndef LITERATURE_H
#define LITERATURE_H

#include <QWidget>
#include <QDir>
#include <QFile>
#include <QTableWidgetItem>
#include <QDesktopServices>
#include <QUrl>

namespace Ui {
class Literature;
}

class Literature : public QWidget
{
    Q_OBJECT

public:
    explicit Literature(QWidget *parent = nullptr);

    void setBooks();

    ~Literature();

private slots:
    void openhtmlfile(QTableWidgetItem *item); //кликнули запуск файла

private:
    void setStyle();

    Ui::Literature *ui;
};

#endif // LITERATURE_H
