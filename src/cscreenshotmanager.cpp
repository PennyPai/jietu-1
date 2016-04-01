#include <QApplication>
#include <QGuiApplication>
#include "cscreenshotmanager.h"
#include "cscreenshotview.h"
#include "cscreenpublic.h"
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
    clearAll();
}

void CScreenShotManager::startScreenShot()
{
    if(m_isRunning)
    {
        return;
    }
    clearAll();
    QList<QScreen *> screens = QApplication::screens();
    int index = 0;
    QList<QRect> rectList = getWindownRectList();
    C_SCREENSHOTSHARED_LOG(QString("all rect count %1").arg(rectList.count()));

    foreach (QScreen *d, screens)
    {
        index++;
        CScreenShotView *view = new CScreenShotView(rectList,d);
        m_viewList.append(view);
        connect(view,SIGNAL(sigStatusChanged(CScreenShotStatus)),
                this,SLOT(onStatusChanged(CScreenShotStatus)));
        connect(view,SIGNAL(sigPreviewItemShow()),
                this,SLOT(onPreviewItemShow()));
        view->startSCreenShot();
        view->raise();
    }
    m_isRunning = true;
}

void CScreenShotManager::clearAll()
{
    //可能为导致crash
    //qDeleteAll(m_viewList);
    foreach (CScreenShotView *v, m_viewList)
    {
       v->setVisible(false);
       v->deleteLater();
    }
    m_viewList.clear();
}

void CScreenShotManager::onStatusChanged(CScreenShotStatus status)
{
    CScreenShotView *view = dynamic_cast<CScreenShotView*>(sender());
    if(view == NULL)
    {
        C_SCREENSHOTSHARED_LOG(QString("view is NULL"));
        return;
    }
    QPixmap pixmap;
    bool isValid = false;
    if(status == CSCREEN_SHOT_STATE_FINISHED)
    {
        pixmap = view->getPixmap();
        isValid = view->isValid();
    }

    CScreenShotView *firstView = NULL;
    foreach (CScreenShotView *d, m_viewList)
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
#ifdef Q_OS_WIN
            firstView->setVisible(false);
#else
            firstView->deleteLater();
            m_viewList.clear();
#endif
        }
        if(status == CSCREEN_SHOT_STATE_FINISHED)
        {
            if(!pixmap.isNull() && isValid)
            {
                emit sigScreenShotPixmapChanged(pixmap);
            }
            else
            {
                C_SCREENSHOTSHARED_LOG(QString("shot is %1valid,pixmap is %2null")
                         .arg(isValid?"":"not")
                         .arg(pixmap.isNull()?"":"not "));
            }
        }
        m_isRunning = false;
    }
}

void CScreenShotManager::onPreviewItemShow()
{
    CScreenShotView *view = dynamic_cast<CScreenShotView*>(sender());
    if(view == NULL)
    {
        C_SCREENSHOTSHARED_LOG(QString("view is NULL"));
        return;
    }
    foreach (CScreenShotView *d, m_viewList)
    {
        if(d != view)
        {
            d->setPreviewItemHidden(true);
        }
    }
}
