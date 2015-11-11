#include <QScreen>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QGraphicsProxyWidget>
#include <QClipboard>
#include <QTimer>
#include <QRgb>
#include <QDesktopWidget>
#include <QApplication>
#include "cscreenshotview.h"
#include "cscreenshotscene.h"
#include "cscreenselectrectitem.h"
#include "cscreeneditortoolbaritem.h"
#include "cscreenrectitem.h"
#include "cscreentooltipitem.h"

CScreenShotView::CScreenShotView(const QList<QRect> &rectList,
                                 QScreen *screen,
                                 QWidget *parent)
    :QGraphicsView(parent)
    ,m_windowRectList(rectList)
    ,m_desktopScreen(screen)
    ,m_screen(NULL)
    ,m_backgroundItem(NULL)
    ,m_selectRectItem(NULL)
    ,m_toolbarItem(NULL)
    ,m_currentRectItem(NULL)
    ,m_tooltipSizeItem(NULL)
    ,m_previewItem(NULL)
    ,m_positionType(CSCREEN_POSITION_TYPE_NOT_CONTAIN)
    ,m_shotStatus(CSCREEN_SHOT_STATE_INITIALIZED)
    ,m_isPressed(false)
    ,m_isLocked(false)
    ,m_isValid(false)
{
    m_screen = new CScreenShotScene(this);
    this->setScene(m_screen);
    QDesktopWidget *pDesktoWidget = QApplication::desktop();
    QRect geometry= screen->geometry();
    LOG_TEST(QString("screen->geometry() (%1,%2,%3,%4)")
             .arg(geometry.x())
             .arg(geometry.y())
             .arg(geometry.width())
             .arg(geometry.height()));
    QPixmap pixmap = screen->grabWindow(pDesktoWidget->winId(),geometry.x()
                                        ,geometry.y(),geometry.width(),geometry.height());
    drawPixmap(pixmap);
    m_backgroundItem = new QGraphicsPixmapItem(m_backgroundPixmap);
    m_screen->addItem(m_backgroundItem);
    this->setGeometry(geometry);
    m_screen->setSceneRect(QRect(0,0,geometry.width(),geometry.height()));
    m_sx = 1.0 * geometry.width() / pixmap.width();
    m_sy = 1.0 * geometry.height() / pixmap.height();
    m_backgroundItem->setScale(m_sx);
    m_selectRectItem = new CScreenSelectRectItem(m_desktopPixmap);
    m_selectRectItem->setScale(m_sx);
    m_selectRectItem->setVisible(false);
    m_screen->addItem(m_selectRectItem);
    //====================
    m_toolbarItem = new CScreenEditorToolbarItem;
    connect(m_toolbarItem,SIGNAL(sigButtonClicked(CScreenButtonType)),
            this,SLOT(onButtonClicked(CScreenButtonType)));
    m_toolbarItem->setVisible(false);
    m_toolbarItem->setZValue(m_selectRectItem->zValue() + 1);
    m_screen->addItem(m_toolbarItem);
    m_tooltipSizeItem = new CScreenTooltipItem;
    m_tooltipSizeItem->setVisible(false);
    m_screen->addItem(m_tooltipSizeItem);
    m_previewItem = new QGraphicsPixmapItem;
    m_previewItem->setVisible(false);
    m_previewItem->setZValue(m_toolbarItem->zValue() + 1);
    m_screen->addItem(m_previewItem);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setStyleSheet("QWidget{border: 0px solid #1880ed;}");
#ifdef Q_OS_MAC
    qApp->installEventFilter(this);
#endif
    QPoint pos = QCursor::pos();
    if(geometry.contains(pos))
    {
        updatePreviewItem(this->mapFromGlobal(pos));
    }
}

CScreenShotView::~CScreenShotView()
{
    this->overrideWindowFlags(m_flags);
    if(m_screen)
    {
        m_screen->clear();
        delete m_screen;
        m_screen = NULL;
    }
}

void CScreenShotView::startSCreenShot()
{
    this->overrideWindowFlags(Qt::ToolTip);
    this->showFullScreen();
}

void CScreenShotView::setLocked(bool locked)
{
    if(m_isLocked == locked)
    {
        return;
    }
    m_isLocked = locked;
}

QPixmap CScreenShotView::getPixmap()
{
    return m_pixmap;
}

bool CScreenShotView::isValid() const
{
    return m_isValid;
}

void CScreenShotView::setPreviewItemHidden(bool isHidden)
{
    m_previewItem->setVisible(!isHidden);
}

QPixmap CScreenShotView::createPixmap(const QRect &rect)
{
    QPixmap pixmap;
    if(m_shotStatus == CSCREEN_SHOT_STATE_SELECTED || m_shotStatus == CSCREEN_SHOT_STATE_EDITED)
    {
        QDesktopWidget *pDesktoWidget = QApplication::desktop();
        QRect geometry= m_desktopScreen->geometry();
        LOG_TEST(QString("screen->geometry() (%1,%2,%3,%4)")
                 .arg(geometry.x())
                 .arg(geometry.y())
                 .arg(geometry.width())
                 .arg(geometry.height()));
        QPixmap desktopPixmap = m_desktopScreen->grabWindow(pDesktoWidget->winId(),geometry.x()
                                                            ,geometry.y(),geometry.width(),geometry.height());
        pixmap = desktopPixmap.copy(rect);
    }
    return pixmap;
}

QString CScreenShotView::getSizeString(const QSize &size) const
{
    return QString("%1 * %2").arg(size.width()).arg(size.height());
}

QRgb CScreenShotView::getPixmapPosRgb(const QPixmap &pixmap, const QPoint &pos)
{
    QPixmap posPixmap = pixmap.copy(pos.x(), pos.y(), 1, 1);
    if (!posPixmap.isNull())
    {
        QImage image = posPixmap.toImage();
        if (!image.isNull())
        {
            if(image.valid(0, 0))
            {
                return image.pixel(0, 0);
            }
        }
    }
    return QRgb();
}

void CScreenShotView::doFinished()
{
    if(m_shotStatus == CSCREEN_SHOT_STATE_SELECTED || m_shotStatus == CSCREEN_SHOT_STATE_EDITED)
    {
        m_selectRectItem->setMovePointHidden(true);
        m_toolbarItem->setVisible(false);
        m_tooltipSizeItem->setVisible(false);
        //延迟获取图片，否则工具栏可能不消失
        QTimer::singleShot(10, this, SLOT(onFinishTimerOut()));
    }
}

QRect CScreenShotView::getMouseOnWindowRect(const QPoint &pos)
{
    QRect rect;
    int min = 0;
    bool isFirst = true;
    foreach (QRect r,m_windowRectList)
    {
        if(r.contains(pos))
        {
            int distance = getRectDistance(r,pos);
            if(isFirst)
            {
                min = distance;
                rect = r;
                isFirst = false;
            }
            else
            {
                if(min > distance)
                {
                    min = distance;
                    rect = r;
                }
            }
        }
    }
    return rect;
}

int CScreenShotView::getRectDistance(const QRect &rect, const QPoint &pos)
{
    if(!rect.contains(pos))
    {
        return -1;
    }
    int distance = 0;
    distance += qAbs(pos.x() - rect.left());
    distance += qAbs(pos.x() - rect.right());
    distance += qAbs(pos.y() - rect.top());
    distance += qAbs(pos.y() - rect.bottom());
    return distance;
}

bool CScreenShotView::event(QEvent *event)
{
#ifdef Q_OS_MAC
    if(event->type() == QEvent::KeyPress
            || event->type() == QEvent::KeyRelease
            || event->type() == QEvent::MouseButtonDblClick
            || event->type() == QEvent::MouseButtonPress
            || event->type() == QEvent::MouseButtonRelease
            || event->type() == QEvent::MouseMove)
    {
        LOG_TEST(QString("EVENT type %1").arg(event->type()));
        if(event->type() == QEvent::MouseMove)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if(mouseEvent)
            {
                mouseMoveEvent(mouseEvent);
                event->accept();
                return true;
            }
        }
        if(event->type() == QEvent::MouseButtonRelease)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            mouseReleaseEvent(mouseEvent);
            event->accept();
            return true;
        }
    }
#endif
    return QGraphicsView::event(event);
}


void CScreenShotView::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
    {
        setShotStatus(CSCREEN_SHOT_STATE_CANCEL);
        event->accept();
        return;
    }
    else if(event->key() == Qt::Key_Enter
            || event->key() == Qt::Key_Return)
    {
        if(m_shotStatus == CSCREEN_SHOT_STATE_EDITED
                || m_shotStatus == CSCREEN_SHOT_STATE_SELECTED)
        {
            doFinished();
            event->accept();
            return;
        }
    }
    if(m_isLocked)
    {
        event->accept();
        return;
    }
    return QGraphicsView::keyPressEvent(event);
}

void CScreenShotView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        CScreenPositionType type = m_selectRectItem->getPostionType(getPointToSelectedItem(event->pos()));
        LOG_TEST(QString("RightButton pos x %1,y %2,type %3").arg(event->pos().x()).arg(event->pos().y()).arg(type));
        setShotStatus(CSCREEN_SHOT_STATE_CANCEL);
        return;
    }
    if(m_isLocked)
    {
        event->accept();
        return;
    }
    if(event->button() == Qt::LeftButton)
    {
        LOG_TEST(QString("x %1,posX %2, xx%3").arg(this->geometry().x())
                 .arg(event->pos().x())
                 .arg(this->mapFromGlobal(event->pos()).x()));
        QRectF toolBarItemRect(m_toolbarItem->pos(),m_toolbarItem->boundingRect().size());
        if(m_toolbarItem->isVisible() && toolBarItemRect.contains(event->pos()))
        {
            return QGraphicsView::mousePressEvent(event);
        }
        m_startPoint = event->pos();
        m_endPoint = event->pos();
        m_selectRect = m_selectRectItem->getSelectRect();
        m_positionType = m_selectRectItem->getPostionType(getPointToSelectedItem(event->pos()));
        LOG_TEST(QString("m_positionType %1").arg(m_positionType));
        bool isContains = (m_positionType != CSCREEN_POSITION_TYPE_NOT_CONTAIN);
        if((isContains
            && m_shotStatus == CSCREEN_SHOT_STATE_SELECTED)
                || m_shotStatus == CSCREEN_SHOT_STATE_INITIALIZED)
        {
            m_isPressed = true;
            m_toolbarItem->setVisible(false);
        }
        else if(m_shotStatus == CSCREEN_SHOT_STATE_EDITED
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
        if(m_shotStatus == CSCREEN_SHOT_STATE_INITIALIZED)
        {
            updatePreviewItem(event->pos());
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
    if(m_isLocked)
    {
        event->accept();
        return;
    }
    QRectF selectRect = m_selectRectItem->getSelectRect();
    if(m_isPressed && (selectRect.width() >= 1 && selectRect.height() >= 1))
    {
        if(m_shotStatus == CSCREEN_SHOT_STATE_INITIALIZED)
        {
            updateToolbarPosition();
            m_selectRect = selectRect;
            setShotStatus(CSCREEN_SHOT_STATE_SELECTED);
            m_previewItem->setVisible(false);
            m_selectRectItem->setVisible(true);
            m_selectRectItem->setMovePointHidden(false);
            m_toolbarItem->setVisible(true);
        }
        else if(m_shotStatus == CSCREEN_SHOT_STATE_SELECTED)
        {
            updateToolbarPosition();
            m_toolbarItem->setVisible(true);
        }
        else if(m_shotStatus == CSCREEN_SHOT_STATE_EDITED)
        {
            m_currentRectItem = NULL;
        }
    }
    else if(m_isPressed)
    {
        m_selectRectItem->setVisible(false);
        m_tooltipSizeItem->setVisible(false);
    }
    m_isPressed = false;
    QGraphicsView::mouseReleaseEvent(event);
}

void CScreenShotView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(m_isLocked)
    {
        event->accept();
        return;
    }
    if(m_shotStatus == CSCREEN_SHOT_STATE_SELECTED || m_shotStatus == CSCREEN_SHOT_STATE_EDITED)
    {
        bool isContains = m_selectRect.contains(getPointToSelectedItem(event->pos()));
        QRect toolbarRect(m_toolbarItem->pos().x(),
                          m_toolbarItem->pos().y(),
                          m_toolbarItem->boundingRect().width(),
                          m_toolbarItem->boundingRect().height());
        if(isContains && !toolbarRect.contains(event->pos()) && m_toolbarItem->isVisible())
        {
            event->accept();
            doFinished();
            return;
        }
    }
    return QGraphicsView::mouseDoubleClickEvent(event);
}

void CScreenShotView::mouseMoveEvent(QMouseEvent *event)
{
    if(m_isLocked)
    {
        event->accept();
        return;
    }
    if((event->buttons() & Qt::LeftButton) && m_isPressed)
    {
        m_selectRectItem->setVisible(true);
        m_endPoint = event->pos();
        QPointF startPoint = getPointToSelectedItem(m_startPoint);
        QPointF endPoint = getPointToSelectedItem(event->pos());
        QPointF maxPoint = getPointToSelectedItem(QPointF(this->geometry().width(),this->geometry().height()));
        if(m_shotStatus == CSCREEN_SHOT_STATE_INITIALIZED)
        {
            if(endPoint.x() > maxPoint.x())
            {
                endPoint.setX(maxPoint.x());
            }
            if(endPoint.y() > maxPoint.y())
            {
                endPoint.setY(maxPoint.y());
            }
            if(endPoint.x() < 0)
            {
                endPoint.setX(0);
            }
            if(endPoint.y() < 0)
            {
                endPoint.setY(0);
            }
            QRectF rect = getPositiveRect(startPoint,endPoint);
            m_selectRectItem->setSelectedRect(rect);
            updateTooltipItem();
        }
        else if(m_shotStatus == CSCREEN_SHOT_STATE_SELECTED)
        {
            updateSelectRect(startPoint,endPoint);
        }
        else if(m_shotStatus == CSCREEN_SHOT_STATE_EDITED && m_currentRectItem)
        {
            QRectF rect = getPositiveRect(m_startPoint,event->pos());
            m_currentRectItem->setPainterRect(rect);
            m_currentRectItem->setVisible(true);
        }
    }
    if(m_shotStatus == CSCREEN_SHOT_STATE_INITIALIZED)
    {
        updatePreviewItem(event->pos());
        if(!m_isPressed && !m_windowRectList.isEmpty())
        {
            QRect rect = getMouseOnWindowRect(event->globalPos());
            QPointF topLeft = getPointToSelectedItem(rect.topLeft());
            QPointF bottomRight = getPointToSelectedItem(rect.bottomRight());
            QRectF selectRect(topLeft,bottomRight);
            if(selectRect.isValid())
            {
                m_selectRectItem->setVisible(true);
                m_selectRectItem->setSelectedRect(selectRect);
            }
        }
    }

    return QGraphicsView::mouseMoveEvent(event);
}

bool CScreenShotView::eventFilter(QObject *obj, QEvent *event)
{
#ifdef Q_OS_MAC
    if(/*obj == qApp && */event->type() == QEvent::KeyPress || this->isVisible())
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if(keyEvent)
        {
            if(keyEvent->key() == Qt::Key_Escape)
            {
                setShotStatus(CSCREEN_SHOT_STATE_CANCEL);
                event->accept();
                return true;
            }
            else if(keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return)
            {
                if(m_shotStatus == CSCREEN_SHOT_STATE_EDITED
                        || m_shotStatus == CSCREEN_SHOT_STATE_SELECTED)
                {
                    doFinished();
                    event->accept();
                    return true;
                }
            }
        }
    }
    if(this->isVisible())
    {
        //弥补界面最下面、最右边差1px的事件范围
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if(mouseEvent && m_desktopScreen)
        {
            QPoint globalPos = mouseEvent->globalPos();
            QRect geometry = m_desktopScreen->geometry();
            bool isContains = globalPos.x() >= geometry.x()
                    && globalPos.x() <= (geometry.x() + geometry.width())
                    && globalPos.y() >= geometry.y()
                    && globalPos.y() <= (geometry.y() + geometry.height());
            bool onBoundary = (globalPos.x() == (geometry.x() + geometry.width()) || globalPos.y() == (geometry.y() + geometry.height()));
            if(isContains && onBoundary)
            {
                switch (event->type())
                {
                case QEvent::MouseButtonPress:
                    mousePressEvent(mouseEvent);
                    break;
                case QEvent::MouseMove:
                    mouseMoveEvent(mouseEvent);
                    break;
                case QEvent::MouseButtonDblClick:
                    mouseDoubleClickEvent(mouseEvent);
                    break;
                default:
                    break;
                }
            }
        }
    }
#endif
    return QGraphicsView::eventFilter(obj,event);
}

CScreenRectItem *CScreenShotView::createRectItem()
{
    QPointF topLeftPos = getPointFromSelectedItem(m_selectRect.topLeft());
    QPointF bottomRightPos = getPointFromSelectedItem(m_selectRect.bottomRight());
    QRectF rect = getPositiveRect(topLeftPos,bottomRightPos);
    CScreenRectItem *item = new CScreenRectItem(rect,QRectF(0,0,0,0));
    item->setLineColor(m_toolbarItem->getColor());
    item->setLineWidth(m_toolbarItem->getLineWidth());
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

QPointF CScreenShotView::getPointToSelectedItem(const QPointF &point)
{
    return QPointF(point.x() / m_sx,point.y() / m_sy);
}

QPointF CScreenShotView::getPointFromSelectedItem(const QPointF &point)
{
    return QPointF(point.x() * m_sx,point.y() * m_sy);
}

QRectF CScreenShotView::getPositiveRect(const QPointF &startPoint, const QPointF &endPoint)
{
    qreal width = endPoint.x() - startPoint.x();
    qreal height = endPoint.y() - startPoint.y();
    QPointF pos;
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
    QSizeF size(qAbs(width),qAbs(height));
    return QRectF(pos,size);
}

void CScreenShotView::updateToolbarPosition()
{
    if(m_desktopScreen == NULL)
    {
        return;
    }
    QPointF topLeftPos = this->getPointFromSelectedItem(m_selectRectItem->getSelectRect().topLeft());
    QPointF bottomRightPos = this->getPointFromSelectedItem(m_selectRectItem->getSelectRect().bottomRight());
    QRectF rect = getPositiveRect(topLeftPos,bottomRightPos);
    qreal x = rect.right() - m_toolbarItem->boundingRect().width();
    if( x < 0)
    {
        x = 0;
    }
    qreal y = rect.bottom() + m_marginSelectedWidthToolbar
            + m_selectRectItem->pen().width() * m_sx;
    if(y + m_toolbarItem->boundingRect().height() > this->height())
    {
        y = rect.bottom() - m_marginSelectedWidthToolbar - m_toolbarItem->boundingRect().height();
    }
    m_toolbarItem->setPos(x,y);
}

void CScreenShotView::updateTooltipItem()
{
    if(m_desktopScreen == NULL)
    {
        return;
    }
    QPointF topLeftPos = this->getPointFromSelectedItem(m_selectRectItem->getSelectRect().topLeft());
    QPointF bottomRightPos = this->getPointFromSelectedItem(m_selectRectItem->getSelectRect().bottomRight());
    QRectF rect = getPositiveRect(topLeftPos,bottomRightPos);
    m_tooltipSizeItem->setText(getSizeString(rect.size().toSize()));
    qreal x = rect.left();
    qreal y = rect.top() - m_tooltipSizeItem->boundingRect().height()
            - m_selectRectItem->pen().width() * m_sx
            - m_marginSelectedWidthTooltip;
    if(y < 0)
    {
        y = 0;
    }
    m_tooltipSizeItem->setPos(x,y);
    m_tooltipSizeItem->setVisible(true);
}

void CScreenShotView::updatePreviewItem(const QPoint &pos)
{
    QPixmap pixmap(m_previewItemWidth,m_previewItemPixmapHeight+m_previewItemTextHeight);
    pixmap.fill(QColor(Qt::white));
    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::TextAntialiasing
                           | QPainter::SmoothPixmapTransform
                           | QPainter::HighQualityAntialiasing
                           | QPainter::NonCosmeticDefaultPen
                           | QPainter::Qt4CompatiblePainting);
    QPointF toItemPos = getPointToSelectedItem(pos);

    qreal scaleSize = 3;
    {//draw preview
        qreal width = m_previewItemWidth / scaleSize;
        qreal height = m_previewItemPixmapHeight / scaleSize;
        qreal x = (toItemPos.x() - 0.5 * width);
        QPointF pos(0,0);
        if(x < 0)
        {
            width = width + x;
            pos.setX(-x * scaleSize);
            x = 0;
        }
        else
        {
            width = qMin(width,m_desktopPixmap.width() - x);
        }
        qreal y = (toItemPos.y() - 0.5 * height);
        if(y < 0)
        {
            height += y;
            pos.setY(-y * scaleSize);
            y = 0;
        }
        else
        {
            height = qMin(height,m_desktopPixmap.height() - y);
        }
        QPixmap pixmap = m_desktopPixmap.copy(x,y,width,height).scaled(width * scaleSize,height * scaleSize);
        painter.drawImage(pos,pixmap.toImage());
        QColor lineColor("#89D4EB");
        lineColor.setAlpha(128);
        QPen pen(lineColor);
        pen.setWidth(m_previewLineWidth);
        painter.setPen(pen);
        QLineF hLine(0,0.5 * m_previewItemPixmapHeight,m_previewItemWidth,0.5 * m_previewItemPixmapHeight);
        painter.drawLine(hLine);
        QLineF vLine(0.5 * m_previewItemWidth,0,0.5 * m_previewItemWidth,m_previewItemPixmapHeight);
        painter.drawLine(vLine);
    }
    QRectF textRect(0,m_previewItemPixmapHeight,m_previewItemWidth,m_previewItemTextHeight);
    {//draw text background
        QBrush textBrush(QColor("#4F4F4F"));
        QPen pen(Qt::NoPen);
        painter.setPen(pen);
        painter.setBrush(textBrush);
        painter.drawRect(textRect);
    }
    {//draw text
        QRgb pointRgb = getPixmapPosRgb(m_desktopPixmap,toItemPos.toPoint());
        QFont font;
        font.setPixelSize(m_previewPointSize);
        painter.setFont(font);
        QColor textColor("#B8B8B8");
        QPen pen(textColor);
        painter.setPen(pen);
        QString text = QString("(%1,%2)\nRGB:(%3,%4,%5)")
                .arg(pos.x())
                .arg(pos.y())
                .arg(qRed(pointRgb))
                .arg(qGreen(pointRgb))
                .arg(qBlue(pointRgb));
        painter.drawText(textRect,Qt::AlignCenter,text);
    }
    m_previewItem->setPixmap(pixmap);
    qreal x = pos.x() - m_previewItemDx - pixmap.width();
    if(x < 0)
    {
        x = pos.x() + m_previewItemDy;
    }
    qreal y = pos.y() + m_previewItemDy;
    if(y + pixmap.height() > this->height())
    {
        y = pos.y() - m_previewItemDy - pixmap.height();
    }
    m_previewItem->setPos(x,y);
    if(!m_previewItem->isVisible())
    {
        emit sigPreviewItemShow();
        m_previewItem->setVisible(true);
    }
}

void CScreenShotView::setShotStatus(CScreenShotStatus status)
{
    if(m_shotStatus != status)
    {
        m_shotStatus = status;
        emit sigStatusChanged(m_shotStatus);
    }
}

void CScreenShotView::updateSelectRect(const QPointF &startPoint, const QPointF &endPoint)
{
    QPointF maxPoint = getPointToSelectedItem(QPointF(this->geometry().width(),this->geometry().height()));
    qreal dx = startPoint.x() - endPoint.x();
    qreal dy = startPoint.y() - endPoint.y();
    qreal maxX = maxPoint.x();
    qreal maxY = maxPoint.y();
    QPointF topLeft;
    QPointF bottomRight;
    switch (m_positionType)
    {
    case CSCREEN_POSITION_TYPE_TOP_LEFT:
        bottomRight = m_selectRect.bottomRight();
        topLeft = m_selectRect.topLeft() - QPointF(dx,dy);
        break;
    case CSCREEN_POSITION_TYPE_TOP_MIDDLE:
        bottomRight = m_selectRect.bottomRight();
        topLeft = m_selectRect.topLeft() - QPointF(0,dy);
        break;
    case CSCREEN_POSITION_TYPE_LEFT_MIDDLE:
        bottomRight = m_selectRect.bottomRight();
        topLeft = m_selectRect.topLeft() - QPointF(dx,0);
        break;
    case CSCREEN_POSITION_TYPE_TOP_RIGHT:
        bottomRight = m_selectRect.bottomLeft();
        topLeft = m_selectRect.topRight() - QPointF(dx,dy);
        break;
    case CSCREEN_POSITION_TYPE_RIGHT_MIDDLE:
        bottomRight = m_selectRect.bottomLeft();
        topLeft = m_selectRect.topRight() - QPointF(dx,0);
        break;
    case CSCREEN_POSITION_TYPE_BOTTOM_LEFT:
        bottomRight = m_selectRect.topRight();
        topLeft = m_selectRect.bottomLeft() - QPointF(dx,dy);
        break;
    case CSCREEN_POSITION_TYPE_BOTTOM_MIDDLE:
        bottomRight = m_selectRect.topRight();
        topLeft = m_selectRect.bottomLeft() - QPointF(0,dy);
        break;
    case CSCREEN_POSITION_TYPE_BOTTOM_RIGHT:
        bottomRight = m_selectRect.topLeft();
        topLeft = m_selectRect.bottomRight() - QPointF(dx,dy);
        break;
    case CSCREEN_POSITION_TYPE_CONTAIN:
    {
        bottomRight = m_selectRect.topLeft() - QPointF(dx,dy);
        topLeft = m_selectRect.bottomRight() - QPointF(dx,dy);
        qreal x = m_selectRect.x() - dx;
        x = qMax(x,0.0);
        x = qMin(x,maxX - m_selectRect.width());
        qreal y = m_selectRect.y() - dy;
        y = qMax(y,0.0);
        y = qMin(y,maxY - m_selectRect.height());
        QRectF rect(x,y,m_selectRect.width(),m_selectRect.height());
        m_selectRectItem->setSelectedRect(rect);
        updateTooltipItem();
        return;
        break;
    }
    default:
        return;
        break;
    }
    if(topLeft.x() < 0)
    {
        topLeft.setX(0);
    }
    else if(topLeft.x() > maxX)
    {
        topLeft.setX(maxX);
    }
    if(topLeft.y() < 0)
    {
        topLeft.setY(0);
    }
    else if(topLeft.y() > maxY)
    {
        topLeft.setY(maxY);
    }
    if(bottomRight.x() < 0)
    {
        bottomRight.setX(0);
    }
    else if(bottomRight.x() > maxX)
    {
        bottomRight.setX(maxX);
    }
    if(bottomRight.y() < 0)
    {
        bottomRight.setY(0);
    }
    else if(bottomRight.y() > maxY)
    {
        bottomRight.setY(maxY);
    }

    QRectF rect = getPositiveRect(topLeft,bottomRight);
    m_selectRectItem->setSelectedRect(rect);
    updateTooltipItem();
}

void CScreenShotView::onButtonClicked(CScreenButtonType type)
{
    LOG_TEST(QString("onButtonClicked type %1").arg(type));
    switch (type)
    {
    case CSCREEN_BUTTON_TYPE_RECT:
        if(m_shotStatus == CSCREEN_SHOT_STATE_SELECTED)
        {
            setShotStatus(CSCREEN_SHOT_STATE_EDITED);
        }
        break;
    case CSCREEN_BUTTON_TYPE_OK:
        doFinished();
        LOG_TEST(QString("CSCREEN_SHOT_STATE_FINISHED type %1").arg(CSCREEN_SHOT_STATE_FINISHED));
        break;
    case CSCREEN_BUTTON_TYPE_CANCLE:
        setShotStatus(CSCREEN_SHOT_STATE_CANCEL);
        break;
    default:
        break;
    }
}

void CScreenShotView::onFinishTimerOut()
{
    QPointF startPos = m_selectRectItem->getSelectRect().topLeft();
    QPointF endPos = m_selectRectItem->getSelectRect().bottomRight();
    QRect rect = getPositiveRect(startPos,endPos).toRect();
    if(rect.width() > 1)
    {
        m_isValid = true;
        m_pixmap = createPixmap(rect);
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setPixmap(m_pixmap);
    }
    else
    {
        m_isValid = false;
    }
    setShotStatus(CSCREEN_SHOT_STATE_FINISHED);
}
