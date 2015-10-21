#ifndef _CSCREENSELECTRECTITEM_H_
#define _CSCREENSELECTRECTITEM_H_

#include <QGraphicsRectItem>
#include "clogsetting.h"

class CScreenSelectRectItem : public QGraphicsRectItem
{
public:
    CScreenSelectRectItem(const QPixmap &desktopPixmap,QGraphicsItem * parent = 0);
    void setSelectedRect(const QRectF &rect);
    void setScale(qreal scale);
    ~CScreenSelectRectItem();

protected:
    //TODO ==START== 为后期选择移动准备
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    //TODO ==END== 为后期选择移动准备

private:
    //TODO ==START== 为后期选择移动准备
    QGraphicsEllipseItem *createEllipseItem(const QRectF &rect);
    void updateEllipseItems();
    void updateEllipseItem(QGraphicsEllipseItem **item,const QRectF &rect);
    //TODO ==END== 为后期选择移动准备

private:
    //TODO ==START== 为后期选择移动准备
    QGraphicsEllipseItem *m_topLeftEllipseItem;
    QGraphicsEllipseItem *m_bottomLeftEllipseItem;
    QGraphicsEllipseItem *m_topMiddleEllipseItem;
    QGraphicsEllipseItem *m_bottomMiddleEllipseItem;
    QGraphicsEllipseItem *m_topRightEllipseItem;
    QGraphicsEllipseItem *m_bottomRightEllipseItem;
    QGraphicsEllipseItem *m_leftMiddleEllipseItem;
    QGraphicsEllipseItem *m_rightMiddleEllipseItem;
    //TODO ==END== 为后期选择移动准备
    QPixmap m_desktopPixmap;
    QColor m_penColor;
    static const int m_ellipseRadius = 3;
    static const int m_ellipseLineWidth = 1;
    static const int m_ellipseOffset = 3;
    static const int m_penWidth = 2;
};

#endif // _CSCREENSELECTRECTITEM_H_

