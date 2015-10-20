#include <QScreen>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include "cscreenshotview.h"
#include "cscreenshotscene.h"
#include "cscreenrectitem.h"

#include <QUuid>
#include <QStandardPaths>
#include <QDebug>
#include <QDesktopWidget>
#include <QApplication>

CScreenShotView::CScreenShotView(QScreen *screen, QWidget *parent)
    :QGraphicsView(parent)
    ,m_screen(NULL)
    ,m_backgroundItem(NULL)
    ,m_shotState(ShotUnselected)
    ,m_positionType(CSCREEN_POSITION_TYPE_NOT_CONTAIN)
{
    //======
    m_isPressed = false;
//    this->setWindowModality(Qt::WindowModal);
    this->overrideWindowFlags(Qt::ToolTip );
//    this->overrideWindowFlags(Qt::Sheet | Qt::Window );
//    this->setWindowOpacity(0.5);
    m_screen = new CScreenShotScene();
    this->setScene(m_screen);
    QDesktopWidget * pDesktoWidget = QApplication::desktop();
    QRect geometry= screen->geometry();
    foreach (QScreen *d, screen->virtualSiblings())
    {
        qDebug()<<"TTT "<<d->geometry();
    }
    QPixmap pixmap = screen->grabWindow(pDesktoWidget->winId(),geometry.x()
                                        ,geometry.y(),geometry.width(),geometry.height());

    drawPixmap(pixmap);
//    m_pixmap = pixmap;
    QString fileName = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)
            .append(QString("/%1.png").arg(QUuid::createUuid().toString()));
//    pixmap.save(fileName);
    qDebug()<<fileName<<geometry<<pixmap.size();
    m_backgroundItem = new QGraphicsPixmapItem(m_backgroundPixmap);
    m_screen->addItem(m_backgroundItem);
    this->setGeometry(geometry);
    qDebug()<<"transform"<<this->transform();
    m_screen->setSceneRect(QRect(0,0,geometry.width(),geometry.height()));
    m_sx = 1.0 * geometry.width() / pixmap.width();
    m_sy = 1.0 * geometry.height() / pixmap.height();
    {//TYPE01
        //    qreal sy = 1.0 * geometry.height() / pixmap.height();
        //    this->scale(sx,sy);
        //    qDebug()<<"transform"<<this->transform();
        //    qreal dx = geometry.width() * (sx - 1) / 2 / sx;
        //    qreal dy = geometry.height() * (sy - 1) / 2 / sy;
        //    m_backgroundItem->moveBy(dx,dy);
    }
    {//TYPE02s
        m_backgroundItem->setScale(m_sx);
    }
    m_selectRectItem = new CScreenRectItem(m_desktopPixmap);
    m_selectRectItem->setScale(m_sx);
    m_selectRectItem->setVisible(false);
    m_screen->addItem(m_selectRectItem);

}

CScreenShotView::~CScreenShotView()
{

}

void CScreenShotView::startSCreenShot()
{
    this->overrideWindowFlags(Qt::ToolTip);
//    this->setWindowOpacity(0.1);
    this->showFullScreen();
    //    this->setWindowOpacity(1);
    this->overrideWindowFlags(Qt::Sheet | Qt::Window );
}

bool CScreenShotView::event(QEvent *event)
{
//    qDebug()<<"TTTTT"<<event->type();
    return QGraphicsView::event(event);
}

void CScreenShotView::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
    {
        emit sigCancel();
    }
}

void CScreenShotView::mousePressEvent(QMouseEvent *event)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__<<"event->button()"<<event->button();
    if(event->button() == Qt::RightButton)
    {
        emit sigCancel();
        return;
    }
    if(event->button() == Qt::LeftButton)
    {
        qDebug()<<Q_FUNC_INFO<<__LINE__<<"event"<<event->pos()<<"m_selectRectItem->rect()"<<m_selectRectItem->rect();
        m_startPoint = event->pos();
//        m_endPoint = m_startPoint;
        m_selectRect = m_selectRectItem->rect();
        if(m_selectRect.contains(getPoint(event->pos())) || m_shotState == ShotUnselected)
        {
            m_isPressed = true;
        }

    }
    else
    {
        m_isPressed = false;
    }
    return QGraphicsView::mousePressEvent(event);
}

void CScreenShotView::mouseReleaseEvent(QMouseEvent *event)
{
//
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    if(/*(event->buttons() & Qt::LeftButton) && */m_isPressed && m_shotState == ShotUnselected)
    {
        qDebug()<<"mouseReleaseEvent2";
        m_shotState = ShotSelect;
//        m_endPoint = m_startPoint;
//        m_selectRect = m_selectRectItem->rect();
    }
//    m_selectRect = m_selectRectItem->rect();
    m_isPressed = false;
}

void CScreenShotView::mouseDoubleClickEvent(QMouseEvent *event)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
//    QRect rect = getPositiveRect(m_startPoint,m_endPoint);
    QRect rect = getPositiveRect(m_selectRectItem->rect().topLeft(),m_endPoint);
    QString fileName = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)
            .append(QString("/%1.png").arg(QUuid::createUuid().toString()));
//    pixmap.save(fileName);
    QPixmap::grabWidget(this,rect).save(fileName);
}

void CScreenShotView::mouseMoveEvent(QMouseEvent *event)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    if((event->buttons() & Qt::LeftButton) && m_isPressed)
    {
        qDebug()<<"event"<<event->pos();
        m_selectRectItem->setVisible(true);
//        QRect geometry = this->geometry();
        m_endPoint = event->pos();
//        if(m_endPoint.x() > geometry.right())
//        {
//            m_endPoint.setX(geometry.right());
//        }
//        else if(m_endPoint.x() < geometry.left())
//        {
//            m_endPoint.setX(geometry.left());
//        }
//        qDebug()<<Q_FUNC_INFO<<__LINE__<<"event->pos()"<<event->pos()<<"geometry"<<geometry<<"new m_endPoint"<<m_endPoint;

        QPointF startPoint = getPoint(m_startPoint);
        QPointF endPoint = getPoint(event->pos());
        QPointF maxPoint = getPoint(QPointF(this->geometry().width(),this->geometry().height()));
        if(m_shotState == ShotUnselected)
        {
            QRect rect = getPositiveRect(startPoint,endPoint);
            m_selectRectItem->setSelectedRect(rect);
        }
        else if(m_shotState == ShotSelect)
        {
            qreal dx = startPoint.x() - endPoint.x();
            qreal dy = startPoint.y() - endPoint.y();
            qreal x = -dx + m_selectRect.x();
            qreal y = -dy + m_selectRect.y();
            qreal maxX = maxPoint.x() - m_selectRect.width();
            qreal maxY = maxPoint.y() - m_selectRect.height();
            if(x < 0)
            {
                x = 0;
            }
            else if(x >  maxX)
            {
                x = maxX;
            }

            if(y < 0)
            {
                y = 0;
            }
            else if(y >  maxY)
            {
                y = maxY;
            }
            QRectF rect(x,y,m_selectRect.width(),m_selectRect.height());
            m_selectRectItem->setSelectedRect(rect);
            qDebug()<<Q_FUNC_INFO<<__LINE__<<"rect"<<rect<<"m_selectRect" << m_selectRect<<"dx"<<dx<<"dy"<<dy;
        }
    }
}

void CScreenShotView::drawPixmap(const QPixmap &pixmap)
{
    m_desktopPixmap = pixmap;
    m_backgroundPixmap = pixmap;
    QPainter painter(&m_backgroundPixmap);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
//    painter.fillRect(m_pixmap.rect(),QColor(50,50,50,50));
    painter.fillRect(m_backgroundPixmap.rect(),QColor(0,0,0,90));
//    m_pixmap = QPixmap::fromImage(image);
//    int penWidth = 2;
//    QPen pen = painter.pen();
//    pen.setWidth(penWidth);
//    painter.setPen(pen);
//    painter.fillRect(m_x,m_y,m_width,m_height,Qt::transparent);
    //    painter.drawRect(m_x - penWidth,m_y - penWidth,m_width + 2 * penWidth,m_height + 2 * penWidth);
}

QPointF CScreenShotView::getPoint(const QPointF &point)
{
    return QPointF(point.x() / m_sx,point.y() / m_sy);
}

QRect CScreenShotView::getPositiveRect(const QPointF &startPoint, const QPointF &endPoint)
{
    qreal width = endPoint.x() - startPoint.x();
    qreal height = endPoint.y() - startPoint.y();
    QPoint pos;
    if(width > 0)
    {
        pos.setX(startPoint.x());
    }
    else
    {
        pos.setX(endPoint.x());
    }
    if(height > 0)
    {
        pos.setY(startPoint.y());
    }
    else
    {
        pos.setY(endPoint.y());
    }
    QSize size(qAbs(width),qAbs(height));
    return QRect(pos,size);
}

