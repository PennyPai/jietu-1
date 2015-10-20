#include <QApplication>
#include <QGuiApplication>
#include "cscreenshotmanager.h"
#include "cscreenshotview.h"
#include <QDebug>
#include <QWindowList>
#include <QWindow>

CScreenShotManager::CScreenShotManager(QObject *parent)
    :QObject(parent)
{

}

CScreenShotManager::~CScreenShotManager()
{
    //
}

void CScreenShotManager::startScreenShot()
{
    m_viewList.clear();
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
        connect(view,SIGNAL(sigCancel()),
                this,SLOT(onCancel()));
        view->startSCreenShot();
//        view->show();
    }

    QWindowList windowList = QGuiApplication::allWindows();

    qDebug()<<"windowList "<<windowList.count();
    foreach (QWindow *w, windowList) {
        qDebug()<<"w "<<w->geometry();
    }
}

void CScreenShotManager::onCancel()
{
    foreach (CScreenShotView *d, m_viewList)
    {
        d->close();
    }
}
