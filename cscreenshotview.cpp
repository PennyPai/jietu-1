#include <QScreen>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QGraphicsProxyWidget>
#include "cscreenshotview.h"
#include "cscreenshotscene.h"
#include "cscreenselectrectitem.h"
#include "cscreeneditorwidget.h"
#include "cscreeneditortoolbaritem.h"

#include <QUuid>
#include <QStandardPaths>
#include <QDebug>
#include <QDesktopWidget>
#include <QApplication>

CScreenShotView::CScreenShotView(const QSharedPointer<CScreenEditorWidget> &screenEditorWidget,
                                 QScreen *screen,
                                 QWidget *parent)
    :QGraphicsView(parent)
    ,m_screen(NULL)
    ,m_backgroundItem(NULL)
    ,m_selectRectItem(NULL)
    ,m_toolbarItem(NULL)
    ,m_currentRectItem(NULL)
    ,m_screenEditorWidget(screenEditorWidget)
    ,m_positionType(CSCREEN_POSITION_TYPE_NOT_CONTAIN)
    ,m_shotState(CSCREEN_SHOT_STATE_INITIALIZED)
{
    //======
    m_isPressed = false;
    //    this->setWindowModality(Qt::WindowModal);
    this->overrideWindowFlags(Qt::ToolTip );
    //    this->overrideWindowFlags(Qt::Sheet | Qt::Window );
    //    this->setWindowOpacity(0.5);
    m_screen = new CScreenShotScene();
    this->setScene(m_screen);
    //    m_screenEditorWidgetItem = new QGraphicsProxyWidget;
    //    QWidget *widget = new CScreenEditorWidget;
    //    m_screenEditorWidgetItem->setWidget(widget);
    //    m_screenEditorWidgetItem->setZValue(111);
    //    m_screen->addItem(m_screenEditorWidgetItem);

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
    m_selectRectItem = new CScreenSelectRectItem(m_desktopPixmap);
    m_selectRectItem->setScale(m_sx);
    m_selectRectItem->setVisible(false);
    m_screen->addItem(m_selectRectItem);
    //====================
    m_toolbarItem = new CScreenEditorToolbarItem;
    connect(m_toolbarItem,SIGNAL(sigButtonClicked(CScreenButtonType)),
            this,SLOT(onButtonClicked(CScreenButtonType)));
    m_toolbarItem->setVisible(false);
    m_screen->addItem(m_toolbarItem);

}

CScreenShotView::~CScreenShotView()
{
    m_screenEditorWidgetItem->setWidget(NULL);
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
    if(event->button() == Qt::RightButton)
    {
        emit sigCancel();
        return;
    }
    if(event->button() == Qt::LeftButton)
    {
        m_startPoint = event->pos();
        m_selectRect = m_selectRectItem->rect();
        bool isContains = m_selectRect.contains(getPoint(event->pos()));
        if((isContains
                && m_shotState == CSCREEN_SHOT_STATE_SELECTED)
                || m_shotState == CSCREEN_SHOT_STATE_INITIALIZED)
        {
            m_isPressed = true;
            m_toolbarItem->setVisible(false);
        }
        else if(m_shotState == CSCREEN_SHOT_STATE_EDITED
                && m_toolbarItem->getCurrentButtonType() == CSCREEN_BUTTON_TYPE_RECT
                && isContains
                && !m_isPressed)
        {
            m_isPressed = true;
            if(m_currentRectItem == NULL)
            {
                m_currentRectItem = createRectItem();
                m_currentRectItem->setVisible(false);
                m_screen->addItem(m_currentRectItem);
            }
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
    if(m_isPressed)
    {
        qDebug()<<"mouseReleaseEvent2";
        if(m_shotState == CSCREEN_SHOT_STATE_INITIALIZED)
        {
            m_shotState = CSCREEN_SHOT_STATE_SELECTED;
        }
        m_selectRect = m_selectRectItem->rect();
        m_toolbarItem->setVisible(true);
        updateToolbarPosition();
    }

    //    m_selectRect = m_selectRectItem->rect();
    m_isPressed = false;

    return QGraphicsView::mouseReleaseEvent(event);
}

void CScreenShotView::mouseDoubleClickEvent(QMouseEvent *event)
{
    //    QRect rect = getPositiveRect(m_startPoint,m_endPoint);
    QRect rect = getPositiveRect(m_selectRectItem->rect().topLeft(),m_endPoint);
    QString fileName = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)
            .append(QString("/%1.png").arg(QUuid::createUuid().toString()));
    //    pixmap.save(fileName);
    QPixmap::grabWidget(this,rect).save(fileName);
    return QGraphicsView::mouseDoubleClickEvent(event);
}

void CScreenShotView::mouseMoveEvent(QMouseEvent *event)
{
    if((event->buttons() & Qt::LeftButton) && m_isPressed)
    {
        m_selectRectItem->setVisible(true);
        m_endPoint = event->pos();
        QPointF startPoint = getPoint(m_startPoint);
        QPointF endPoint = getPoint(event->pos());
        QPointF maxPoint = getPoint(QPointF(this->geometry().width(),this->geometry().height()));
        if(m_shotState == CSCREEN_SHOT_STATE_INITIALIZED)
        {
            QRect rect = getPositiveRect(startPoint,endPoint);
            m_selectRectItem->setSelectedRect(rect);
        }
        else if(m_shotState == CSCREEN_SHOT_STATE_SELECTED)
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
            else if(x > maxX)
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
        }
        else if(m_shotState == CSCREEN_SHOT_STATE_EDITED && m_currentRectItem)
        {
            QRect rect = getPositiveRect(m_startPoint,event->pos());
            m_currentRectItem->setRect(rect);
            m_currentRectItem->setVisible(true);
        }
    }
    return QGraphicsView::mouseMoveEvent(event);
}

QGraphicsRectItem *CScreenShotView::createRectItem()
{
    QGraphicsRectItem *item = new QGraphicsRectItem;
    QPen pen;
    pen.setWidth(1);
    pen.setColor(QColor(Qt::red));
    item->setPen(pen);
//    item->setZValue(-1);
    return item;
}

void CScreenShotView::drawPixmap(const QPixmap &pixmap)
{
    m_desktopPixmap = pixmap;
    m_backgroundPixmap = pixmap;
    QPainter painter(&m_backgroundPixmap);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.fillRect(m_backgroundPixmap.rect(),QColor(0,0,0,90));
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

void CScreenShotView::updateToolbarPosition()
{
    qreal x = m_selectRectItem->rect().left() * m_sx;
    qreal y = m_selectRectItem->rect().bottom() * m_sx + m_marginSelectedWidthToolbar;
    m_toolbarItem->setPos(x,y);
}

void CScreenShotView::onButtonClicked(CScreenButtonType type)
{
    LOG_TEST(QString("onButtonClicked type %1").arg(type));
    switch (type)
    {
    case CSCREEN_BUTTON_TYPE_RECT:
        if(m_shotState == CSCREEN_SHOT_STATE_SELECTED)
        {
            m_shotState = CSCREEN_SHOT_STATE_EDITED;
        }
        break;
    case CSCREEN_BUTTON_TYPE_OK:
        break;
    case CSCREEN_BUTTON_TYPE_CANCLE:
        emit sigCancel();
        break;
    default:
        break;
    }
}

