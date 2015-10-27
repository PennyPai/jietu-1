#ifndef _CSCREENATTRIBUTETOOLBARITEM_H_
#define _CSCREENATTRIBUTETOOLBARITEM_H_

#include <QGraphicsObject>
#include "clogsetting.h"
#include "cscreenpublic.h"

class CScreenEditorButtonItem;
class CScreenColorToolbarItem;

class CScreenAttributeToolbarItem : public QGraphicsObject
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    CScreenAttributeToolbarItem(QGraphicsItem *parent = 0);
    ~CScreenAttributeToolbarItem();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    void setLineAttributeHidden(bool isHidden);
    void setArrowDirectionPosition(CScreenArrowDirection direction,const qreal dx);
    int getLineWidth() const;
    QColor getColor() const;

private:
    CScreenEditorButtonItem *createLineWidthItem(const QImage &icon,const QImage &pressedBackground);
    void updateLayout();
    QImage createColorItemBackground(const QColor &color);

private slots:
    void onItemClicked();
    void onCurrentColorChanged(const QColor &color);

private:
    CScreenEditorButtonItem *m_lineWidthSmallButtonItem;
    CScreenEditorButtonItem *m_lineWidthMiddleButtonItem;
    CScreenEditorButtonItem *m_lineWidthLargeButtonItem;
    CScreenEditorButtonItem *m_colorButtonItem;
    CScreenColorToolbarItem *m_colorToolbarItem;
    QRectF m_rect;
    CScreenArrowDirection m_arrowDirection;
    int m_lineWidth;
    QColor m_color;
    qreal m_arrawDx;
    static const int m_marginLineWidthItem = 4;
    static const int m_marginLineWidthWithColorItem = 11;
    static const int m_marginTop = 6;
    static const int m_marginLeft = 6;
    static const int m_marginRight = 10;
    static const int m_marginBottom = 6;

    static const int m_lineItemWidth = 23;
    static const int m_lineItemHeight = 23;
    static const int m_colorItemWidth = 58;
    static const int m_colorItemHeight = 23;
    static const int m_triangleHeight = 8;
    static const int m_triangleWidth = 10;

    static const int m_marginBottomColorToolbarToView = 20;
    static const int m_lineWidthSmall = 3;
    static const int m_lineWidthMiddle = 6;
    static const int m_lineWidthLarge = 9;

};

#endif // _CSCREENATTRIBUTETOOLBARITEM_H_
