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
        painter->drawImage(m_rect,QImage(":/screenshot/res/screenshot/buttonpressedbg.png"));
        painter->drawImage(m_rect,m_imagePressed);
    }
    else if(m_isHover)
    {
        painter->drawImage(m_rect,QImage(":/screenshot/res/screenshot/buttonhoverbg.png"));
        painter->drawImage(m_rect,m_imageNormal);
    }
    else
    {
        painter->drawImage(m_rect,m_imageNormal);
    }

}

void CScreenEditorButtonItem::clearSelected()
{
    if(m_isPressed)
    {
        m_isPressed = false;
        update();
    }
}

void CScreenEditorButtonItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_isPressed = true;
        event->accept();
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
