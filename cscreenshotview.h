#ifndef CSCREENSHOTVIEW
#define CSCREENSHOTVIEW

#include <QGraphicsView>

#include "cscreenpublic.h"

class CScreenShotScene;
class CScreenRectItem;

class CScreenShotView : public QGraphicsView
{
    Q_OBJECT
public:
    CScreenShotView(QScreen *screen,QWidget *parent = 0);
    ~CScreenShotView();
    void startSCreenShot();

protected:
    bool event(QEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    void drawPixmap(const QPixmap &pixmap);
    QPointF getPoint(const QPointF &point);
    QRect getPositiveRect(const QPointF &startPoint,const QPointF &endPoint);
//    void getAdjustEndPoint(const QRectF &rect,const QPointF &startPoint,QPointF &endPoint);

signals:
    void sigCancel();

private:
    enum ShotState{ShotUnselected,ShotSelect,ShotEdited};
    CScreenShotScene *m_screen;
    QGraphicsPixmapItem *m_backgroundItem;
    CScreenRectItem *m_selectRectItem;
    ShotState m_shotState;
    CScreenPositionType m_positionType;
    //====
    bool m_isPressed;
    QPoint m_startPoint;
    QPoint m_endPoint;
    QPixmap m_backgroundPixmap;
    QPixmap m_desktopPixmap;
    qreal m_sx;
    qreal m_sy;
    QRectF m_selectRect;
};

#endif // CSCREENSHOTVIEW

