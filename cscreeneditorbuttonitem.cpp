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
//    QPen pen(Qt::NoPen);
//    QPen pen;
//    QColor color(Qt::red);
//    pen.setColor(color);
//    painter->setPen(pen);
//    painter->drawRect(m_rect);
//    painter->drawRect(QRect(0,0,111,666));
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
    qDebug()<<Q_FUNC_INFO<<__LINE__<<"mousePressEvent";
    if(event->button() == Qt::LeftButton)
    {
        m_isPressed = true;
        emit sigClicked();
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
//    if(m_isPressed)
    qDebug()<<Q_FUNC_INFO<<__LINE__<<"mouseReleaseEvent";

    {
        m_isPressed = false;
        update();
    }
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
    qDebug()<<Q_FUNC_INFO<<__LINE__<<"mouseDoubleClickEvent";
    return QGraphicsObject::mouseDoubleClickEvent(event);
}
