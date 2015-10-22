#include <QApplication>
#include <QGuiApplication>
#include "cscreenshotmanager.h"
#include "cscreenshotview.h"
#include <QDebug>
#include <QWindowList>
#include <QWindow>

CScreenShotManager * CScreenShotManager::m_instance = NULL;

CScreenShotManager::CScreenShotManager(QObject *parent)
    :QObject(parent)
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
    clearAll();
    QList<QScreen *> screens = QApplication::screens();
    int index = 0;
    foreach (QScreen *d, screens)
    {
        index++;
        if(index % 2 == 0)
        {
//            continue;
        }
        CScreenShotView *view = new CScreenShotView(d);
        m_viewList.append(view);
        connect(view,SIGNAL(sigStatusChanged(CScreenShotStatus)),
                this,SLOT(onStatusChanged(CScreenShotStatus)));
        view->startSCreenShot();
//        view->show();
    }
}

void CScreenShotManager::clearAll()
{
    qDeleteAll(m_viewList);
    m_viewList.clear();
}

void CScreenShotManager::onStatusChanged(CScreenShotStatus status)
{
    CScreenShotView *view = dynamic_cast<CScreenShotView*>(sender());
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

    foreach (CScreenShotView *d, m_viewList)
    {
        if(status == CSCREEN_SHOT_STATE_CANCEL || status == CSCREEN_SHOT_STATE_FINISHED)
        {
            d->setHidden(true);
        }
        else
        {
            if(view != d)
            {
                d->setLocked(true);
            }
        }
    }
    if(status == CSCREEN_SHOT_STATE_FINISHED)
    {
        emit sigScreenShotPixmapChanged(pixmap);
//        clearAll();
    }
    else if(status == CSCREEN_SHOT_STATE_CANCEL)
    {
//        clearAll();
    }
}
