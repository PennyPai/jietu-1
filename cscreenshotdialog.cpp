#include <QHBoxLayout>
#include <QScreen>
#include <QEvent>
#include "cscreenshotdialog.h"
#include "cscreenshotview.h"

CScreenShotDialog::CScreenShotDialog(QScreen *screen, QWidget *parent, Qt::WindowFlags f)
    :QDialog(parent,f)
{
    m_view  =  new CScreenShotView(screen);
    connect(m_view,SIGNAL(sigPreviewItemShow()),this,SIGNAL(sigPreviewItemShow()));
    connect(m_view,SIGNAL(sigStatusChanged(CScreenShotStatus)),this,SIGNAL(sigStatusChanged(CScreenShotStatus)));
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
    //不设置TOOLTIP 可以接受键盘事件
    this->overrideWindowFlags(Qt::ToolTip);
    this->setModal(true);
    this->showFullScreen();
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

bool CScreenShotDialog::event(QEvent *e)
{
    LOG_TEST(QString("event type %1").arg(e->type()));
    return QDialog::event(e);
}
