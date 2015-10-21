#ifndef _CSCREENRECTITEM_H_
#define _CSCREENRECTITEM_H_
#include <QGraphicsObject>
#include "clogsetting.h"

class CScreenRectItem : public QGraphicsObject
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    CScreenRectItem(const QRectF &rect,const QRectF &painterRect,QGraphicsItem *parent = 0);
    ~CScreenRectItem();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    void setPainterRect(const QRectF &rect);

private:
    QPixmap createBackgroupdPixmap();

private:
    QRectF m_rect;
    QRectF m_painterRect;
};

#endif // _CSCREENRECTITEM_H_

