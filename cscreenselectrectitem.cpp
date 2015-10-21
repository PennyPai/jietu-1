#include <QPen>
#include <QBrush>
#include <QGraphicsSceneMouseEvent>
#include "cscreenselectrectitem.h"
#include <QDebug>

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
    , m_desktopPixmap(desktopPixmap)
    , m_penColor(QColor("#00ACFF"))
{
    QBrush brush(m_desktopPixmap);
    this->setBrush(brush);

    m_penColor.setAlpha(128);
    QPen pen(m_penColor);
    pen.setWidth(m_penWidth);
    this->setPen(pen);
    this->setAcceptedMouseButtons(Qt::LeftButton);
//    this->setFlag(QGraphicsItem::ItemIsMovable,true);
}

void CScreenSelectRectItem::setSelectedRect(const QRectF &rect)
{
    this->setRect(rect);
    updateEllipseItems();
}

void CScreenSelectRectItem::setScale(qreal scale)
{
    if(qAbs(scale) < 0.00000000001)
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
}

CScreenSelectRectItem::~CScreenSelectRectItem()
{

}

void CScreenSelectRectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    return QGraphicsRectItem::mousePressEvent(event);
}

void CScreenSelectRectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    return QGraphicsRectItem::mouseMoveEvent(event);
}

void CScreenSelectRectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    return QGraphicsRectItem::mouseReleaseEvent(event);
}

QGraphicsEllipseItem *CScreenSelectRectItem::createEllipseItem(const QRectF &rect)
{
    QGraphicsEllipseItem *item = new QGraphicsEllipseItem(this);
    item->setZValue(this->zValue() + 1);
    item->setRect(rect);
    QPen pen(Qt::red);
    pen.setWidth(1);
    item->setPen(pen);
    //TODO
    item->setVisible(false);
    return item;
}

void CScreenSelectRectItem::updateEllipseItems()
{
    QRectF rect = this->rect();
    qDebug()<<Q_FUNC_INFO<<__LINE__<<"rect"<<rect<<"rect.bottomLeft()"<<rect.bottomLeft()<<rect.bottomRight();
    qreal radius = m_ellipseRadius / this->scale();
    qreal diameter = 2 * radius;
    qreal leftPosX = rect.left() - radius;
    qreal rightPosX = rect.right() - radius;
    qreal hMiddlePosX = rect.center().x() - radius;
    qreal topPosY = rect.top() - radius;
    qreal bottomPosY = rect.bottom() - radius;
    qreal vMiddlePosY = rect.center().y() - radius;

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
    if(((*item) == NULL))
    {
        *item = createEllipseItem(rect);
        return;
    }
    else
    {
        (*item)->setRect(rect);
    }
}
