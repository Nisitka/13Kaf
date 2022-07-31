#include "winerror.h"
#include "ui_winerror.h"

#include <QPixmap>

winError::winError(
                   QString textError,
                   QString textButton,
                   QWidget *parent) :
    QWidget(parent),
    ui(new Ui::winError)
{
    ui->setupUi(this);

    ui->imageLabel->setMaximumWidth(120);

    this->setWindowTitle("Ошибка!");
    ui->pushButton->setStyleSheet("QPushButton"
                                  "{"
                                  "     border: 1px solid black;"
                                  "     border-radius: 5px;"
                                  "}"
                                  "QPushButton:hover"
                                  "{"
                                  "     background: rgb(255, 255, 255);"
                                  "     border: 1px solid black;"
                                  "     border-radius: 5px;"
                                  "}"
                                  "QPushButton:pressed"
                                  "{"
                                  "     background: rgb(180, 180, 180);"
                                  "     border: 1px solid black;"
                                  "     border-radius: 5px;"
                                  "}"
                                  );

    connect(
            ui->pushButton, SIGNAL(clicked(bool)),
            this, SLOT(hide())
            );

    ui->label->setText(textError);
    ui->imageLabel->setPixmap(QCoreApplication::applicationDirPath() + "\\" +
                         nameResurses + "\\" +
                         "exit.png");

    ui->pushButton->setText(textButton);

    this->show();
}

void winError::hideEvent(QHideEvent* e)
{

}

winError::~winError()
{
    delete ui;
}
