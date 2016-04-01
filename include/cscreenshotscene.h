#ifndef _CSCREENSHOTSCENE_H_
#define _CSCREENSHOTSCENE_H_

#include <QGraphicsScene>
#include "cscreenshot_global.h"

class CSCREENSHOTSHARED_EXPORT CScreenShotScene : public QGraphicsScene
{
    Q_OBJECT

public:
    CScreenShotScene(QObject *parent = 0);
    ~CScreenShotScene();
};

#endif // _CSCREENSHOTSCENE_H_

