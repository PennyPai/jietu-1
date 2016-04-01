#ifndef _CSCREENRECTITEM_H_
#define _CSCREENRECTITEM_H_
#include <QGraphicsObject>
#include "cscreenshot_global.h"

class CSCREENSHOTSHARED_EXPORT CScreenRectItem : public QGraphicsObject
{
    Q_OBJECT

public:
    CScreenRectItem(const QRectF &rect,const QRectF &painterRect,QGraphicsItem *parent = 0);
    ~CScreenRectItem();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    void setPainterRect(const QRectF &rect);
    void setLineWidth(int width);
    void setLineColor(const QColor &color);

private:
    QPixmap createBackgroupdPixmap();

private:
    QRectF m_rect;
    QRectF m_painterRect;
    int m_lineWidth;
    QColor m_lineColor;
};

#endif // _CSCREENRECTITEM_H_

