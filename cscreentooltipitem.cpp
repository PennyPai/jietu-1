#include <QPainter>
#include <QPen>
#include <QBrush>
#include "cscreentooltipitem.h"

CScreenTooltipItem::CScreenTooltipItem(QGraphicsItem *parent)
    :QGraphicsObject(parent)
    ,m_rect(0,0,82,19)
    ,m_colorBackgroud("#d3d3d3")
    ,m_colorText("#ffffff")
{
    this->setOpacity(0.8);
    m_text = QString("121 * 123");
}



CScreenTooltipItem::~CScreenTooltipItem()
{

}

QRectF CScreenTooltipItem::boundingRect() const
{
    return m_rect;
}

void CScreenTooltipItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    {
        QPen pen;
        pen.setColor(m_colorBackgroud);
        painter->setPen(pen);
        QBrush brush(m_colorBackgroud);
        painter->setBrush(brush);
        painter->drawRoundedRect(m_rect,m_rectRadius,m_rectRadius);
    }
    {
        QPen pen;
        pen.setColor(m_colorText);
        painter->setPen(pen);
        QFont font;
//        font.setPixelSize(m_textPointSize);
        font.setPointSize(m_textPointSize);
        painter->setFont(font);
        painter->drawText(m_rect,Qt::AlignCenter, m_text);
    }
}

void CScreenTooltipItem::setText(const QString &text)
{
    if(m_text == text)
    {
        return;
    }
    prepareGeometryChange();
    m_text = text;
    update();
}
