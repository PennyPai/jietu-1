#include <QApplication>
#include <QGuiApplication>
#include "cscreenshotmanager.h"
#include "cscreenshotview.h"
#include <QDebug>
#include <QWindowList>
#include <QWindow>

QList<QRect> c_global_windown_rect_list;		//用于存储所有的窗口
#ifdef Q_OS_WIN
#include <Windows.h>
#include <vector>
//声明回调函数
bool CALLBACK getWindownRect(HWND hwnd,LPARAM lParam);

bool CALLBACK getWindownRect(HWND hwnd,LPARAM lParam)
{
	if (IsWindow(hwnd) && IsWindowVisible(hwnd) && !IsIconic(hwnd))
	{
		RECT tempRect;
		QRect rect;
		::GetWindowRect(hwnd,&tempRect);
		rect.setTopLeft(QPoint(tempRect.left,tempRect.top));
        //QRect 差一个px
		rect.setBottomRight(QPoint(tempRect.right - 1,tempRect.bottom - 1));
        if(!c_global_windown_rect_list.contains(rect))
        {
            c_global_windown_rect_list.append(rect);
        }
        //遍历子窗口,TODO
//		::EnumChildWindows(hwnd,(WNDENUMPROC)updateWindowRectList,0);
	}
	return true;
}
#endif

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
        return;
    }
    clearAll();
    QList<QScreen *> screens = QApplication::screens();
    int index = 0;
#ifdef Q_OS_WIN
    c_global_windown_rect_list.clear();
    //遍历所有顶级窗口,更新窗口大小列表
    ::EnumWindows((WNDENUMPROC)getWindownRect,0);
#endif
    LOG_TEST(QString("all rect count %1").arg(c_global_windown_rect_list.count()));
    foreach (QScreen *d, screens)
    {
        index++;
        CScreenShotView *view = new CScreenShotView(c_global_windown_rect_list,d);
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
        LOG_WARNING(QString("view is NULL"));
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
        if(status == CSCREEN_SHOT_STATE_FINISHED && !pixmap.isNull() && isValid)
        {
            emit sigScreenShotPixmapChanged(pixmap);
        }
        m_isRunning = false;
    }
}

void CScreenShotManager::onPreviewItemShow()
{
    CScreenShotView *view = dynamic_cast<CScreenShotView*>(sender());
    if(view == NULL)
    {
        LOG_WARNING(QString("view is NULL"));
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
