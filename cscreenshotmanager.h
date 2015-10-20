#ifndef CSCREENSHOTMANAGER
#define CSCREENSHOTMANAGER

#include <QObject>

class CScreenShotView;

class CScreenShotManager : public QObject
{
    Q_OBJECT
public:
    explicit CScreenShotManager(QObject *parent=0);
    ~CScreenShotManager();
    void startScreenShot();

private slots:
    void onCancel();

private:
    QList<CScreenShotView *> m_viewList;
};

#endif // CSCREENSHOTMANAGER

