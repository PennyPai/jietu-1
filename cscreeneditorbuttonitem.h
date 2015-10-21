#ifndef _CSCREENEDITORBUTTONITEM_H_
#define _CSCREENEDITORBUTTONITEM_H_
#include <QGraphicsObject>
#include "clogsetting.h"

class CScreenEditorButtonItem : public QGraphicsObject
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    CScreenEditorButtonItem(const QImage &imageNormal,const QImage &imagePressed,const QRectF &rect,QGraphicsItem *parent = 0);
    ~CScreenEditorButtonItem();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    void clearSelected();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

signals:
    void sigClicked();

private:
    bool m_isHover;
    bool m_isPressed;
    QImage m_imageNormal;
    QImage m_imagePressed;
    QRectF m_rect;
};

#endif // _CSCREENEDITORBUTTONITEM_H_

