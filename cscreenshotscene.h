#ifndef CSCREENSHOTSCENE
#define CSCREENSHOTSCENE

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

#endif // CSCREENSHOTSCENE

