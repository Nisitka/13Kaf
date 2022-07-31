#ifndef WINERROR_H
#define WINERROR_H

#include <QWidget>

namespace Ui {
class winError;
}

class winError : public QWidget
{
    Q_OBJECT

public:
    winError(QString textError, QString textButton, QWidget *parent = 0);
    ~winError();

    void hideEvent(QHideEvent*);

private:
    QString nameResurses = "resursImage";

    Ui::winError *ui;
};

#endif // WINERROR_H
