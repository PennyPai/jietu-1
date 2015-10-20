#ifndef CSCREENSHOTSCENE
#define CSCREENSHOTSCENE

#include <QGraphicsScene>

class CScreenShotScene : public QGraphicsScene
{
    Q_OBJECT
public:
    CScreenShotScene(QObject *parent = 0);
    ~CScreenShotScene();
};

#endif // CSCREENSHOTSCENE

