#include <QPainter>
#include <QPixmap>
#include <QGraphicsScene>
#include "cscreenrectitem.h"

CScreenRectItem::CScreenRectItem(const QRectF &rect, const QRectF &painterRect, QGraphicsItem *parent)
    :QGraphicsObject(parent)
    ,m_rect(rect)
    ,m_painterRect(painterRect)
    ,m_lineWidth(1)
{
    //
}

CScreenRectItem::~CScreenRectItem()
{

}

QRectF CScreenRectItem::boundingRect() const
{
    return m_rect;
}

void CScreenRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    QImage image = createBackgroupdPixmap().toImage();
    painter->drawImage(m_rect.left(),m_rect.top(),image);
    painter->restore();
}

void CScreenRectItem::setPainterRect(const QRectF &rect)
{
    prepareGeometryChange();
    m_painterRect = rect;
    update();
}

void CScreenRectItem::setLineWidth(int width)
{
    if(m_lineWidth == width)
    {
        return;
    }
    m_lineWidth = width;
    update();
}

void CScreenRectItem::setLineColor(const QColor &color)
{
    if(m_lineColor == color)
    {
        return;
    }
    m_lineColor = color;
    update();
}

QPixmap CScreenRectItem::createBackgroupdPixmap()
{
    qreal x = m_painterRect.x() - m_rect.x();
    qreal y = m_painterRect.y() - m_rect.y();
    QPixmap pixmap(m_rect.size().toSize());
    pixmap.fill(QColor(0,0,0,0));
    QPainter painter(&pixmap);
    QPen pen;
    pen.setColor(m_lineColor);
    pen.setWidth(m_lineWidth);
    painter.setPen(pen);
    painter.drawRect(QRect(x,y,m_painterRect.width(),m_painterRect.height()));
    return pixmap;
}
