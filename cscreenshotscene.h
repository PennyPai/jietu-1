#ifndef _CSCREENSHOTSCENE_H_
#define _CSCREENSHOTSCENE_H_

#include <QGraphicsScene>
#include "clogsetting.h"

class CScreenShotScene : public QGraphicsScene
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    CScreenShotScene(QObject *parent = 0);
    ~CScreenShotScene();
};

#endif // _CSCREENSHOTSCENE_H_

