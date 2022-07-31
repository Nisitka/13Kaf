#include "menu.h"
#include "ui_menu.h"

#include "sizeImage.h"

#include <QDebug>

#include <QTimer>

#include "winerror.h"

menu::menu(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::menu)
{
    ui->setupUi(this);

    this->showFullScreen();

    connect(
            ui->exitButton, SIGNAL(clicked()),
            this, SLOT(close())
            );
    connect(
            ui->exitButton, SIGNAL(clicked()),
            this, SIGNAL(closeApp())
            );

    connect(
            ui->testsButton, SIGNAL(clicked()),
            this, SLOT(clickedTesting())
            );
    connect(
            ui->usersButton, SIGNAL(clicked()),
            this, SLOT(clickedUsers())
            );
    connect(
            ui->rezulButton, SIGNAL(clicked()),
            this, SLOT(clickedRezult())
            );

    QTimer *timer = new QTimer(this);
          connect(timer, SIGNAL(timeout()), this, SLOT(nextImage()));
          timer->start(4000);
    index = 0;

    timerSetingsButton = new QTimer(this);
                connect(timerSetingsButton, SIGNAL(timeout()), this, SLOT(anSettingButton()));
    indexSettingsButton = 0;

    timerUsersButton = new QTimer(this);
                connect(timerUsersButton, SIGNAL(timeout()), this, SLOT(anUsersButton()));
    indexUsersButton = 0;

    setStyle();
    setImagesBox();

    connectDB = false;

    setMouseTracking(true); //чтобы события перемещения мыши возникали без зажатия кнопки

}



//движение мыши
void menu::mouseMoveEvent(QMouseEvent* mouseEvent)
{
    //ui->nameProgramLabel->setText(QString::number(mouseEvent->x()) + " " + QString::number(mouseEvent->y()));
    int x = mouseEvent->x();
    int y = mouseEvent->y();

    //кнопка настроек
    if (
        x < ui->setingsButton->pos().x() + ui->setingsButton->width() + 5 and
        x > ui->setingsButton->pos().x() - 5 and

        y < ui->setingsButton->pos().y() + ui->setingsButton->height() + 5 and
        y > ui->setingsButton->pos().y() - 5
        )
    {
        timerSetingsButton->start(150);
    }
    else
    {
        timerSetingsButton->stop();
    }
    //кнопка выбора пользователя
    if (
        x < ui->usersButton->pos().x() + ui->usersButton->width() + 5 and
        x > ui->usersButton->pos().x() - 5 and

        y < ui->usersButton->pos().y() + ui->usersButton->height() + 5 and
        y > ui->usersButton->pos().y() - 5
        )
    {
        timerUsersButton->start(500);
        ui->usersButton->setStyleSheet("QPushButton"
                                      "{"
                                      "     background: rgb(128, 255, 53);"
                                      "     border: 1px solid black;"
                                      "     border-radius: 5px;"
                                      "}"
                                       );
    }
    else
    {
        timerUsersButton->stop();
        QPixmap pixIcon = QPixmap(
                           QCoreApplication::applicationDirPath() + "\\" +
                           nameResurses + "\\" +
                           "Users.png"
                           );
        ui->usersButton->setStyleSheet("QPushButton"
                                      "{"
                                      "     background: rgb(128, 210, 53);"
                                      "     border: 1px solid black;"
                                      "     border-radius: 5px;"
                                      "}"
                                       );
        ui->usersButton->setIcon(QIcon(pixIcon));
        ui->usersButton->setIconSize(QSize(40, 40));
    }
}

void menu::anUsersButton() //128 255 0 - зеленый выделения
{
    QPixmap pixIcon;
    if (indexUsersButton == 4)
    {
        indexUsersButton = 0;
    }
    switch (indexUsersButton) {
    case 0:
        pixIcon = QPixmap(
                           QCoreApplication::applicationDirPath() + "\\" +
                           nameResurses + "\\" +
                           "Users0.png"
                           );
        ui->usersButton->setIcon(QIcon(pixIcon));
        ui->usersButton->setIconSize(QSize(40, 40));
        break;
    case 1:
        pixIcon = QPixmap(
                           QCoreApplication::applicationDirPath() + "\\" +
                           nameResurses + "\\" +
                           "Users1.png"
                           );
        ui->usersButton->setIcon(QIcon(pixIcon));
        ui->usersButton->setIconSize(QSize(40, 40));
        break;
    case 2:
        pixIcon = QPixmap(
                           QCoreApplication::applicationDirPath() + "\\" +
                           nameResurses + "\\" +
                           "Users2.png"
                           );
        ui->usersButton->setIcon(QIcon(pixIcon));
        ui->usersButton->setIconSize(QSize(40, 40));
        break;
    case 3:
        pixIcon = QPixmap(
                           QCoreApplication::applicationDirPath() + "\\" +
                           nameResurses + "\\" +
                           "Users1.png"
                           );
        ui->usersButton->setIcon(QIcon(pixIcon));
        ui->usersButton->setIconSize(QSize(40, 40));
        break;
    }
    indexUsersButton++;
}

void menu::anSettingButton()
{
    QPixmap pixIcon;
    if (indexSettingsButton == 4)
    {
        indexSettingsButton = 0;
    }
    switch (indexSettingsButton) {
    case 0:
        pixIcon = QPixmap(
                           QCoreApplication::applicationDirPath() + "\\" +
                           nameResurses + "\\" +
                           "Settings.png"
                           );
        ui->setingsButton->setIcon(QIcon(pixIcon));
        ui->setingsButton->setIconSize(QSize(40, 40));
        break;
    case 1:
        pixIcon = QPixmap(
                           QCoreApplication::applicationDirPath() + "\\" +
                           nameResurses + "\\" +
                           "Settings2.png"
                           );
        ui->setingsButton->setIcon(QIcon(pixIcon));
        ui->setingsButton->setIconSize(QSize(40, 40));
        break;
    case 2:
        pixIcon = QPixmap(
                           QCoreApplication::applicationDirPath() + "\\" +
                           nameResurses + "\\" +
                           "Settings3.png"
                           );
        ui->setingsButton->setIcon(QIcon(pixIcon));
        ui->setingsButton->setIconSize(QSize(40, 40));
        break;
    case 3:
        pixIcon = QPixmap(
                           QCoreApplication::applicationDirPath() + "\\" +
                           nameResurses + "\\" +
                           "Settings4.png"
                           );
        ui->setingsButton->setIcon(QIcon(pixIcon));
        ui->setingsButton->setIconSize(QSize(40, 40));
        break;
    }
    indexSettingsButton++;
}

void menu::setSetings(Setings* s)
{
    setings = s;
}

//открыть тесты
void menu::clickedTesting()
{
    if (connectDB)
    {
        emit buildTableTest();
        emit openTesting();
    }
    else
    {
        if (setings->getConnectDB())
        {
            emit buildTableTest();
            emit openTesting();

            connectDB = true;
        }
        else
        {
            new winError("База данных не подключена!", "ОК");
        }
    }
}

void menu::clickedUsers()
{
    if (connectDB)
    {
        emit openUsers();
    }
    else
    {
        if (setings->getConnectDB())
        {
            emit openUsers();
            connectDB = true;
        }
        else
        {
            new winError("База данных не подключена!", "ОК");
        }
    }
}

void menu::clickedRezult()
{
    if (connectDB)
    {
        emit openRezult();
    }
    else
    {
        if (setings->getConnectDB())
        {
            emit openRezult();
            connectDB = true;
        }
        else
        {
            new winError("База данных не подключена!", "ОК");
        }
    }
}

void menu::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    QPixmap pix(
                QCoreApplication::applicationDirPath() + "\\" +
                nameResurses + "\\" +
                "fonMenu.jpg"
                );
    //фон главного меню
    QRect rect(QPoint(0, 0), this->size());
    painter.drawPixmap(rect, pix);

    //очередная картинка по центру
    QPixmap image(
                QCoreApplication::applicationDirPath() + "\\" +
                nameResurses + "\\" + nameStackedImages + "\\" +
                list_jpg[index]
                  );
    double p = 0.6; //доля картинки от всего экранаы
    QSize sizeImage = setSizeImage(image.size(), QSize(this->size().width()*p, this->size().height()*p));
    QRect rectImage(QPoint((this->size().width()-sizeImage.width())/2, (this->size().height()-sizeImage.height())/2), sizeImage);
    painter.drawPixmap(rectImage, image);

    //рамка
    painter.setPen(QPen(QColor(64,66,68),3, Qt::SolidLine));
    painter.drawRect(rectImage);

    //все сгладить
    painter.setRenderHint(QPainter::Antialiasing, true);
}

void menu::setImagesBox()
{
    //зачитываем имена всех картинок
    QString dirname_jpg = QCoreApplication::applicationDirPath() + "/" +
            nameResurses + "/" + nameStackedImages;

    QDir dir2(dirname_jpg);
    list_jpg = dir2.entryList(QStringList() << "*.jpg");
    numStackedImage = list_jpg.size();
}

void menu::nextImage()
{
    index = rand() % numStackedImage;
    //для вызова QPaintEvent
    this->repaint();
}

void menu::setStyle()
{
    QPixmap* pixIcon;
    ui->exitButton->setStyleSheet("QPushButton"
                                  "{"
                                  "     background: rgb(210, 47, 30);"
                                  "     border: 1px solid black;"
                                  "     border-radius: 5px;"
                                  "}"
                                  "QPushButton:hover"
                                  "{"
                                  "     background: rgb(255, 47, 0);"
                                  "     border: 1px solid rgb(100, 0, 15);"
                                  "     border-radius: 5px;"
                                  "}"
                                  );

    //кнопка литературы
    ui->literButton->setStyleSheet("QPushButton"
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
                                  "QPushButton:pressed"
                                  "{"
                                  "     background: rgb(88, 170, 53);"
                                  "     border: 1px solid black;"
                                  "     border-radius: 5px;"
                                  "}"
                                  );
    pixIcon = new QPixmap(
                       QCoreApplication::applicationDirPath() + "\\" +
                       nameResurses + "\\" +
                       "Book.png"
                       );
    ui->literButton->setIcon(QIcon(*pixIcon));
    ui->literButton->setIconSize(QSize(45, 45));

    //кнопка результатов
    ui->rezulButton->setStyleSheet("QPushButton"
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
                                  "QPushButton:pressed"
                                  "{"
                                  "     background: rgb(88, 170, 53);"
                                  "     border: 1px solid black;"
                                  "     border-radius: 5px;"
                                  "}"
                                  );
    pixIcon = new QPixmap(
                       QCoreApplication::applicationDirPath() + "\\" +
                       nameResurses + "\\" +
                       "grafiks.png"
                       );
    ui->rezulButton->setIcon(QIcon(*pixIcon));
    ui->rezulButton->setIconSize(QSize(50, 50));

    //кнопка настроек
    ui->setingsButton->setStyleSheet("QPushButton"
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
                                  "QPushButton:pressed"
                                  "{"
                                  "     background: rgb(88, 170, 53);"
                                  "     border: 1px solid black;"
                                  "     border-radius: 5px;"
                                  "}"
                                  );
    pixIcon = new QPixmap(
                       QCoreApplication::applicationDirPath() + "\\" +
                       nameResurses + "\\" +
                       "Settings.png"
                       );
    ui->setingsButton->setIcon(QIcon(*pixIcon));
    ui->setingsButton->setIconSize(QSize(40, 40));

    //кнопка тестов
    ui->testsButton->setStyleSheet("QPushButton"
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
                                  "QPushButton:pressed"
                                  "{"
                                  "     background: rgb(88, 170, 53);"
                                  "     border: 1px solid black;"
                                  "     border-radius: 5px;"
                                  "}"
                                  );
    pixIcon = new QPixmap(
                       QCoreApplication::applicationDirPath() + "\\" +
                       nameResurses + "\\" +
                       "Help.png"
                       );
    ui->testsButton->setIcon(QIcon(*pixIcon));
    ui->testsButton->setIconSize(QSize(40, 40));

    //кнопка выбора пользователя
    ui->usersButton->setStyleSheet("QPushButton"
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
                                  "QPushButton:pressed"
                                  "{"
                                  "     background: rgb(88, 170, 53);"
                                  "     border: 1px solid black;"
                                  "     border-radius: 5px;"
                                  "}"
                                  );
    pixIcon = new QPixmap(
                       QCoreApplication::applicationDirPath() + "\\" +
                       nameResurses + "\\" +
                       "Users.png"
                       );
    ui->usersButton->setIcon(QIcon(*pixIcon));
    ui->usersButton->setIconSize(QSize(40, 40));

    ui->nameProgramLabel->setStyleSheet("color: rgb(128, 255, 0);");
                                        //"background: rgb(0, 0, 0);");
    ui->label->setStyleSheet("color: rgb(255, 255, 255);"
                             "border-radius: 5px;"
                             "background: rgb(0, 0, 0, 200);");
}

QPushButton* menu::getLinerButton()
{
    return ui->literButton;
}
QPushButton* menu::getSetingsButton()
{
    return ui->setingsButton;
}
QPushButton* menu::getTestingButton()
{
    return ui->testsButton;
}
QPushButton* menu::getRezultButton()
{
    return ui->rezulButton;
}
QPushButton* menu::getUsersButton()
{
    return ui->usersButton;
}

menu::~menu()
{
    delete ui;
}

