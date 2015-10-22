#ifndef _CSCREENSHOTMANAGER_H_
#define _CSCREENSHOTMANAGER_H_

#include <QObject>
#include <QSharedPointer>
#include "cscreenpublic.h"
#include "clogsetting.h"

class CScreenShotView;

class CScreenShotManager : public QObject
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    static CScreenShotManager *getInstance();
    ~CScreenShotManager();
    void startScreenShot();
    void clearAll();

protected:
    explicit CScreenShotManager(QObject *parent=0);

private slots:
    void onStatusChanged(CScreenShotStatus status);

signals:
    void sigScreenShotPixmapChanged(const QPixmap &pixmap);

private:
    static CScreenShotManager *m_instance;
    QList<CScreenShotView *> m_viewList;
};

#endif // _CSCREENSHOTMANAGER_H_

