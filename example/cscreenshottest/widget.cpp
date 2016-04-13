#include "widget.h"
#include "ui_widget.h"
#include "cscreenshotmanager.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(CScreenShotManager::getInstance(),SIGNAL(sigScreenShotPixmapChanged(QPixmap)),
            this,SLOT(onScreenShotPixmapChanged(QPixmap)));
//    ui->labelPixmap->setScaledContents(true);
//    this->setAttribute(Qt::WA_QuitOnClose,false);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::onScreenShotPixmapChanged(const QPixmap &pixmap)
{
    QPixmap tempPixmap = pixmap;
    tempPixmap.setDevicePixelRatio(ui->labelPixmap->devicePixelRatioF());
    ui->labelPixmap->setPixmap(pixmap);
}

void Widget::on_pushButton_2_clicked()
{
    this->close();
}

void Widget::on_pushButton_clicked()
{
    //
}

void Widget::on_btnShot_clicked()
{
    CScreenShotManager::getInstance()->startScreenShot();
}
