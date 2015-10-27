#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include "cscreeneditorbuttonitem.h"
#include <QDebug>

CScreenEditorButtonItem::CScreenEditorButtonItem(const QImage &imageNormal, const QImage &imagePressed, const QRectF &rect, QGraphicsItem *parent)
    :QGraphicsObject(parent)
    ,m_isHover(false)
    ,m_isPressed(false)
    ,m_imageNormal(imageNormal)
    ,m_imagePressed(imagePressed)
    ,m_imageNormalBackground(QImage())
    ,m_imageHoverBackground(":/screenshot/res/screenshot/buttonhoverbg.png")
    ,m_imagePressedBackground(":/screenshot/res/screenshot/buttonpressedbg.png")
    ,m_rect(rect)
{
    this->setAcceptHoverEvents(true);
}

CScreenEditorButtonItem::~CScreenEditorButtonItem()
{

}

QRectF CScreenEditorButtonItem::boundingRect() const
{
    return m_rect;
}

void CScreenEditorButtonItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen(Qt::NoPen);
    painter->setPen(pen);
    if(m_isPressed)
    {
        painter->drawImage(m_rect,m_imagePressedBackground);
        painter->drawImage(m_rect,m_imagePressed);
    }
    else if(m_isHover)
    {
        painter->drawImage(m_rect,m_imageHoverBackground);
        painter->drawImage(m_rect,m_imageNormal);
    }
    else
    {
        painter->drawImage(m_rect,m_imageNormalBackground);
        painter->drawImage(m_rect,m_imageNormal);
    }

}

void CScreenEditorButtonItem::setSelected(bool isSelected)
{
    if(m_isPressed != isSelected)
    {
        m_isPressed = isSelected;
        update();
    }
}

void CScreenEditorButtonItem::setIsHover(bool isHover)
{
    if(m_isHover != isHover)
    {
        m_isHover = isHover;
        update();
    }
}

void CScreenEditorButtonItem::setNormalBackground(const QImage &image)
{
    if(m_imageNormalBackground == image)
    {
        return;
    }
    m_imageNormalBackground = image;
    update();
}

void CScreenEditorButtonItem::setHoverBackground(const QImage &image)
{
    if(m_imageHoverBackground == image)
    {
        return;
    }
    m_imageHoverBackground = image;
    update();
}

void CScreenEditorButtonItem::setPressedBackground(const QImage &image)
{
    if(m_imagePressedBackground == image)
    {
        return;
    }
    m_imagePressedBackground = image;
    update();
}

void CScreenEditorButtonItem::setNormalIcon(const QImage &image)
{
    if(m_imageNormal == image)
    {
        return;
    }
    m_imageNormal = image;
    update();
}

void CScreenEditorButtonItem::setPressedIcon(const QImage &image)
{
    if(m_imagePressed == image)
    {
        return;
    }
    m_imagePressed = image;
    update();
}

void CScreenEditorButtonItem::setRect(const QRectF &rect)
{
    if(m_rect == rect)
    {
        return;
    }
    m_rect = rect;
    update();
}

void CScreenEditorButtonItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_isPressed = true;
        event->accept();
        update();
        emit sigClicked();
        return;
    }
    else
    {
        m_isPressed = false;
    }
    update();
    QGraphicsObject::mousePressEvent(event);
}

void CScreenEditorButtonItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    LOG_TEST(QString("mouseReleaseEvent"));
    QGraphicsObject::mouseReleaseEvent(event);
}

void CScreenEditorButtonItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    m_isHover = true;
    update();
    QGraphicsObject::hoverEnterEvent(event);
}

void CScreenEditorButtonItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_isHover = false;
    update();
    QGraphicsObject::hoverLeaveEvent(event);
}

void CScreenEditorButtonItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    LOG_TEST(QString("mouseDoubleClickEvent"));
    return QGraphicsObject::mouseDoubleClickEvent(event);
}
