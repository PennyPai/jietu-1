#ifndef _CSCREENSHOTDIALOG_H_
#define _CSCREENSHOTDIALOG_H_

#include <QDialog>
#include "cscreenpublic.h"
#include "clogsetting.h"
class CScreenShotView;

class CScreenShotDialog : public QDialog
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
    
public:
    CScreenShotDialog(QScreen *screen, QWidget *parent = 0, Qt::WindowFlags f = 0);
   ~CScreenShotDialog();
    void startSCreenShot();
    void setLocked(bool locked);
    QPixmap getPixmap();
    void setPreviewItemHidden(bool isHidden);

protected:
    bool event(QEvent *e);

signals:
    void sigStatusChanged(CScreenShotStatus status);
    void sigPreviewItemShow();
    
private:
    CScreenShotView *m_view;
};

#endif // _CSCREENSHOTDIALOG_H_
