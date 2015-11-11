#include <QPen>
#include <QBrush>
#include <QGraphicsSceneMouseEvent>
#include "cscreenselectrectitem.h"
#include "cscreenpublic.h"

CScreenSelectRectItem::CScreenSelectRectItem(const QPixmap &desktopPixmap, QGraphicsItem *parent)
    : QGraphicsRectItem(parent)
    , m_topLeftEllipseItem(NULL)
    , m_bottomLeftEllipseItem(NULL)
    , m_topMiddleEllipseItem(NULL)
    , m_bottomMiddleEllipseItem(NULL)
    , m_topRightEllipseItem(NULL)
    , m_bottomRightEllipseItem(NULL)
    , m_leftMiddleEllipseItem(NULL)
    , m_rightMiddleEllipseItem(NULL)
    , m_movePointHidden(true)
    , m_desktopPixmap(desktopPixmap)
    , m_penColor(QColor("#77d7ff"))
    , m_rect(0,0,0,0)
{
    QBrush brush(m_desktopPixmap);
    this->setBrush(brush);

//    m_penColor.setAlpha(128);
    QPen pen(m_penColor);
    pen.setWidth(m_penWidth);
    this->setPen(pen);
    this->setAcceptedMouseButtons(Qt::LeftButton);
}

void CScreenSelectRectItem::setSelectedRect(const QRectF &rect)
{
    m_rect = rect;
    qreal penWidth = this->pen().width() * 0.5;
    QRect adjustRect(m_rect.x() - penWidth,
                     m_rect.y() - penWidth,
                     m_rect.width() + 2 * penWidth,
                     m_rect.height() + 2 * penWidth);
    QGraphicsRectItem::setRect(adjustRect);
    updateEllipseItems();
}

void CScreenSelectRectItem::setScale(qreal scale)
{
    if(qAbs(scale) < C_SCREEN_ZERO)
    {
        return;
    }
    if(scale < 0)
    {
        return;
    }
    QPen pen(m_penColor);
    pen.setWidth(m_penWidth / scale);
    this->setPen(pen);
    QGraphicsRectItem::setScale(scale);
    setSelectedRect(m_rect);
}

CScreenSelectRectItem::~CScreenSelectRectItem()
{

}

QRectF CScreenSelectRectItem::getSelectRect() const
{
    return m_rect;
}

void CScreenSelectRectItem::setMovePointHidden(bool hidden)
{
    if(m_movePointHidden == hidden)
    {
        return;
    }
    m_movePointHidden = hidden;
    if(m_topLeftEllipseItem)
    {
        m_topLeftEllipseItem->setVisible(!m_movePointHidden);
    }
    if(m_bottomLeftEllipseItem)
    {
        m_bottomLeftEllipseItem->setVisible(!m_movePointHidden);
    }
    if(m_topMiddleEllipseItem)
    {
        m_topMiddleEllipseItem->setVisible(!m_movePointHidden);
    }
    if(m_bottomMiddleEllipseItem)
    {
        m_bottomMiddleEllipseItem->setVisible(!m_movePointHidden);
    }
    if(m_topRightEllipseItem)
    {
        m_topRightEllipseItem->setVisible(!m_movePointHidden);
    }
    if(m_bottomRightEllipseItem)
    {
        m_bottomRightEllipseItem->setVisible(!m_movePointHidden);
    }
    if(m_leftMiddleEllipseItem)
    {
        m_leftMiddleEllipseItem->setVisible(!m_movePointHidden);
    }
    if(m_rightMiddleEllipseItem)
    {
        m_rightMiddleEllipseItem->setVisible(!m_movePointHidden);
    }
    update();
}

CScreenPositionType CScreenSelectRectItem::getPostionType(const QPointF &pos)
{
    if(!m_movePointHidden)
    {
        if(m_topLeftEllipseItem)
        {
            if(m_topLeftEllipseItem->rect().contains(pos))
            {
                return CSCREEN_POSITION_TYPE_TOP_LEFT;
            }
        }
        if(m_bottomLeftEllipseItem)
        {
            if(m_bottomLeftEllipseItem->rect().contains(pos))
            {
                return CSCREEN_POSITION_TYPE_BOTTOM_LEFT;
            }
        }
        if(m_topMiddleEllipseItem)
        {
            if(m_topMiddleEllipseItem->rect().contains(pos))
            {
                return CSCREEN_POSITION_TYPE_TOP_MIDDLE;
            }
        }
        if(m_bottomMiddleEllipseItem)
        {
            if(m_bottomMiddleEllipseItem->rect().contains(pos))
            {
                return CSCREEN_POSITION_TYPE_BOTTOM_MIDDLE;
            }
        }
        if(m_topRightEllipseItem)
        {
            if(m_topRightEllipseItem->rect().contains(pos))
            {
                return CSCREEN_POSITION_TYPE_TOP_RIGHT;
            }
        }
        if(m_bottomRightEllipseItem)
        {
            if(m_bottomRightEllipseItem->rect().contains(pos))
            {
                return CSCREEN_POSITION_TYPE_BOTTOM_RIGHT;
            }
        }
        if(m_leftMiddleEllipseItem)
        {
            if(m_leftMiddleEllipseItem->rect().contains(pos))
            {
                return CSCREEN_POSITION_TYPE_LEFT_MIDDLE;
            }
        }
        if(m_rightMiddleEllipseItem)
        {
            if(m_rightMiddleEllipseItem->rect().contains(pos))
            {
                return CSCREEN_POSITION_TYPE_RIGHT_MIDDLE;
            }
        }
    }
    if(m_rect.contains(pos))
    {
        return CSCREEN_POSITION_TYPE_CONTAIN;
    }
    return CSCREEN_POSITION_TYPE_NOT_CONTAIN;
}

void CScreenSelectRectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    return QGraphicsRectItem::mousePressEvent(event);
}

void CScreenSelectRectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    return QGraphicsRectItem::mouseMoveEvent(event);
}

void CScreenSelectRectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    return QGraphicsRectItem::mouseReleaseEvent(event);
}

QGraphicsEllipseItem *CScreenSelectRectItem::createEllipseItem(const QRectF &rect)
{
    QGraphicsEllipseItem *item = new QGraphicsEllipseItem(this);
    item->setZValue(this->zValue() + 1);
    item->setRect(rect);
    QColor color("#c9f6ff");
    color.setAlpha(128);
    QPen pen(color);
    pen.setWidth(1);
    item->setPen(pen);
    QBrush brush(Qt::white);
    item->setBrush(brush);
    item->setVisible(!m_movePointHidden);
    return item;
}

void CScreenSelectRectItem::updateEllipseItems()
{
    qreal radius = m_ellipseRadius / this->scale();
    qreal diameter = 2 * radius;
    qreal penWidth = this->pen().width();
    QRect adjustRect(m_rect.x() - penWidth,
                     m_rect.y() - penWidth,
                     m_rect.width() + 2 * penWidth,
                     m_rect.height() + 2 * penWidth);

    qreal leftPosX = adjustRect.left() - radius;
    qreal rightPosX = adjustRect.right() - radius;
    qreal hMiddlePosX = adjustRect.center().x() - radius;
    qreal topPosY = adjustRect.top() - radius;
    qreal bottomPosY = adjustRect.bottom() - radius;
    qreal vMiddlePosY = adjustRect.center().y() - radius;

    QRectF topLeft = QRectF(leftPosX,topPosY,diameter,diameter);
    QRectF leftMiddle = QRectF(leftPosX,vMiddlePosY,diameter,diameter);
    QRectF bottomLeft = QRectF(leftPosX,bottomPosY,diameter,diameter);

    QRectF topRight = QRectF(rightPosX,topPosY,diameter,diameter);
    QRectF rightMiddle = QRectF(rightPosX,vMiddlePosY,diameter,diameter);
    QRectF bottomRight = QRectF(rightPosX,bottomPosY,diameter,diameter);

    QRectF topMiddle = QRectF(hMiddlePosX,topPosY,diameter,diameter);
    QRectF bottomMiddle = QRectF(hMiddlePosX,bottomPosY,diameter,diameter);

    updateEllipseItem(&m_topLeftEllipseItem,topLeft);
    updateEllipseItem(&m_topMiddleEllipseItem,topMiddle);
    updateEllipseItem(&m_topRightEllipseItem,topRight);
    updateEllipseItem(&m_bottomLeftEllipseItem,bottomLeft);
    updateEllipseItem(&m_bottomMiddleEllipseItem,bottomMiddle);
    updateEllipseItem(&m_bottomRightEllipseItem,bottomRight);
    updateEllipseItem(&m_leftMiddleEllipseItem,leftMiddle);
    updateEllipseItem(&m_rightMiddleEllipseItem,rightMiddle);
}

void CScreenSelectRectItem::updateEllipseItem(QGraphicsEllipseItem **item, const QRectF &rect)
{
    if((*item) == NULL)
    {
        *item = createEllipseItem(rect);
        return;
    }
    else
    {
        (*item)->setRect(rect);
    }
}
