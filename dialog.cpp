#include "dialog.h"
#include "ui_dialog.h"
#include "cscreenshotmanager.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
//    this->showMinimized();
    CScreenShotManager::getInstance()->startScreenShot();
}

void Dialog::on_pushButton_2_clicked()
{
//    this->showFullScreen();
    this->showMinimized();
    on_pushButton_clicked();
}
