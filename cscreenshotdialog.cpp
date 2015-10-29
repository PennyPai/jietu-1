#include <QHBoxLayout>
#include <QScreen>
#include "cscreenshotdialog.h"
#include "cscreenshotview.h"

CScreenShotDialog::CScreenShotDialog(QScreen *screen, QWidget *parent, Qt::WindowFlags f)
    :QDialog(parent,f)
{
    m_view  =  new CScreenShotView(screen);
    connect(m_view,SIGNAL(sigPreviewItemShow()),this,SIGNAL(sigPreviewItemShow()));
    connect(m_view,SIGNAL(sigStatusChanged(CScreenShotStatus)),this,SIGNAL(sigStatusChanged(CScreenShotStatus)));
//    this->layout()->add//Widget(m_view);
    QHBoxLayout * layout = new QHBoxLayout;
    layout->addWidget(m_view);
    layout->setMargin(0);
    layout->setContentsMargins(0,0,0,0);
    this->setLayout(layout);
    
    QRect geometry= screen->geometry();
    LOG_TEST(QString("screen->geometry() (%1,%2,%3,%4)")
             .arg(geometry.x())
             .arg(geometry.y())
             .arg(geometry.width())
             .arg(geometry.height()));
    this->setGeometry(geometry);
}

CScreenShotDialog::~CScreenShotDialog()
{
    
}

void CScreenShotDialog::startSCreenShot()
{
    this->overrideWindowFlags(Qt::ToolTip);
//    this->showFullScreen();
    this->exec();
}

void CScreenShotDialog::setLocked(bool locked)
{
    m_view->setLocked(locked);
}

QPixmap CScreenShotDialog::getPixmap()
{
    return m_view->getPixmap();
}

void CScreenShotDialog::setPreviewItemHidden(bool isHidden)
{
    m_view->setPreviewItemHidden(isHidden);
}
