#include <QApplication>
#include <QGuiApplication>
#include "cscreenshotmanager.h"
#include "CScreenShotDialog.h"
#include <QDebug>
#include <QWindowList>
#include <QWindow>

CScreenShotManager * CScreenShotManager::m_instance = NULL;

CScreenShotManager::CScreenShotManager(QObject *parent)
    :QObject(parent)
    ,m_isRunning(false)
{
}

CScreenShotManager *CScreenShotManager::getInstance()
{
    if(m_instance == NULL)
    {
        m_instance = new CScreenShotManager;
    }
    return m_instance;
}

CScreenShotManager::~CScreenShotManager()
{
    //
}

void CScreenShotManager::startScreenShot()
{
    if(m_isRunning)
    {
        LOG_TEST(QString("screen shot  is runing"))
        return;
    }
    clearAll();
    QList<QScreen *> screens = QApplication::screens();
    if(screens.isEmpty())
    {
        return;
    }
    m_isRunning = true;
    
    int index = 0;
    foreach (QScreen *d, screens)
    {
        index++;
        CScreenShotDialog *view = new CScreenShotDialog(d);
        m_viewList.append(view);
        connect(view,SIGNAL(sigStatusChanged(CScreenShotStatus)),
                this,SLOT(onStatusChanged(CScreenShotStatus)));
        connect(view,SIGNAL(sigPreviewItemShow()),
                this,SLOT(onPreviewItemShow()));
        view->startSCreenShot();
    }
}

void CScreenShotManager::clearAll()
{
    //qDeleteAll(m_viewList);
    foreach (CScreenShotDialog *v, m_viewList)
    {
       v->setVisible(false);
       v->deleteLater();
    }
    m_viewList.clear();
}

void CScreenShotManager::onStatusChanged(CScreenShotStatus status)
{
    CScreenShotDialog *view = dynamic_cast<CScreenShotDialog*>(sender());
    if(view == NULL)
    {
        LOG_WARNING(QString("view is NULL"));
        return;
    }
    QPixmap pixmap;
    if(status == CSCREEN_SHOT_STATE_FINISHED)
    {
        pixmap = view->getPixmap();
    }

    CScreenShotDialog *firstView = NULL;
    foreach (CScreenShotDialog *d, m_viewList)
    {
        if(status == CSCREEN_SHOT_STATE_CANCEL || status == CSCREEN_SHOT_STATE_FINISHED)
        {
            if(firstView != NULL)
            {
                //START 防止MAC dock栏隐藏,且必须当前主界面是最后关闭===
                //方式1
//                d->setWindowOpacity(0.01);
//                d->showMinimized();
                //方式2
                d->setVisible(false);
                d->setWindowState((d->windowState() & ~Qt::WindowActive) | Qt::WindowMinimized);
                //END=======================
                d->accept();
#ifdef Q_OS_WIN
            firstView->setVisible(false);
#else
            firstView->deleteLater();
#endif
            }
            else
            {
                firstView = d;
            }
        }
        else
        {
            if(view != d)
            {
                d->setLocked(true);
            }
        }
    }

    if(status == CSCREEN_SHOT_STATE_CANCEL || status == CSCREEN_SHOT_STATE_FINISHED)
    {
        if(firstView)
        {
             //START 防止MAC dock栏隐藏,且必须当前主界面是最后关闭===
            firstView->setWindowOpacity(0.01);
            firstView->showMinimized();
            firstView->accept();
            
#ifdef Q_OS_WIN
            firstView->setVisible(false);
#else
            firstView->deleteLater();
            m_viewList.clear();
#endif
        }
        if(status == CSCREEN_SHOT_STATE_FINISHED)
        {
            emit sigScreenShotPixmapChanged(pixmap);
        }
        m_isRunning = false;
    }
}

void CScreenShotManager::onPreviewItemShow()
{
    CScreenShotDialog *view = dynamic_cast<CScreenShotDialog*>(sender());
    if(view == NULL)
    {
        LOG_WARNING(QString("view is NULL"));
        return;
    }
    foreach (CScreenShotDialog *d, m_viewList)
    {
        if(d != view)
        {
            d->setPreviewItemHidden(true);
        }
    }
}
