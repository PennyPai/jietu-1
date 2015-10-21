#ifndef CSCREENSHOTMANAGER
#define CSCREENSHOTMANAGER

#include <QObject>
#include <QSharedPointer>
#include "clogsetting.h"

class CScreenShotView;
class CScreenEditorWidget;

class CScreenShotManager : public QObject
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    explicit CScreenShotManager(QObject *parent=0);
    ~CScreenShotManager();
    void startScreenShot();
    QSharedPointer<CScreenEditorWidget> getEditorWidget();

private slots:
    void onCancel();

private:
    QList<CScreenShotView *> m_viewList;
    QSharedPointer<CScreenEditorWidget> m_screenEditorWidget;
};

#endif // CSCREENSHOTMANAGER

