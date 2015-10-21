#ifndef CSCREENSHOTVIEW
#define CSCREENSHOTVIEW

#include <QGraphicsView>

#include "cscreenpublic.h"
#include "cscreeneditortoolbaritem.h"
#include "clogsetting.h"

class CScreenShotScene;
class CScreenSelectRectItem;
class CScreenEditorWidget;
class CScreenEditorToolbarItem;

class CScreenShotView : public QGraphicsView
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    CScreenShotView(const QSharedPointer<CScreenEditorWidget> &screenEditorWidget,QScreen *screen,QWidget *parent = 0);
    ~CScreenShotView();
    void startSCreenShot();

protected:
    bool event(QEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    QGraphicsRectItem *createRectItem();

private:
    void drawPixmap(const QPixmap &pixmap);
    QPointF getPoint(const QPointF &point);
    QRect getPositiveRect(const QPointF &startPoint,const QPointF &endPoint);
//    void getAdjustEndPoint(const QRectF &rect,const QPointF &startPoint,QPointF &endPoint);
    void updateToolbarPosition();


private slots:
    void onButtonClicked(CScreenButtonType type);

signals:
    void sigCancel();

private:
    CScreenShotScene *m_screen;
    QGraphicsPixmapItem *m_backgroundItem;
    CScreenSelectRectItem *m_selectRectItem;
    QGraphicsProxyWidget *m_screenEditorWidgetItem;
    CScreenEditorToolbarItem *m_toolbarItem;
    QGraphicsRectItem *m_currentRectItem;

    QSharedPointer<CScreenEditorWidget> m_screenEditorWidget;
    CScreenShotState m_shotState;
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
//    QList<QGraphicsRectItem*> m_editorItemList;
    static const int m_marginSelectedWidthToolbar = 4;
};

#endif // CSCREENSHOTVIEW

