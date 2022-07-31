#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QPushButton>

#include <QPainter>
#include <QImage>
#include <QPixmap>

#include <QMessageBox>

#include <QEvent>
#include <QMouseEvent>

#include "setings.h"

QT_BEGIN_NAMESPACE
namespace Ui { class menu; }
QT_END_NAMESPACE

class menu : public QWidget
{
    Q_OBJECT

signals:
    void openTesting();
    void openUsers();
    void openRezult();

    void buildTableTest();

    void closeApp();

public:
    void paintEvent(QPaintEvent*);
    menu(QWidget *parent = nullptr);

    QPushButton* getLinerButton();
    QPushButton* getSetingsButton();
    QPushButton* getTestingButton();
    QPushButton* getRezultButton();
    QPushButton* getUsersButton();

    void setSetings(Setings*);

    ~menu();

protected:
    void mouseMoveEvent(QMouseEvent* mouseEvent);

private slots:
    void clickedTesting();
    void clickedUsers();
    void clickedRezult();

    void nextImage();

    void anSettingButton();
    void anUsersButton();

public slots:

private:
    Setings* setings;

    QStringList list_jpg;
    int numStackedImage;
    int index;

    void setStyle();

    bool connectDB;

    QString nameResurses = "resursImage";
    QString nameStackedImages = "StackedImage";

    void setImagesBox();

    int indexSettingsButton;
    int indexUsersButton;

    QTimer* timerSetingsButton;
    QTimer* timerUsersButton;

    Ui::menu *ui;
};
#endif // MENU_H
