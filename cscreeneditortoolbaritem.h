#ifndef _CSCREENEDITORTOOLBARITEM_H_
#define _CSCREENEDITORTOOLBARITEM_H_

#include <QGraphicsObject>
#include "cscreenpublic.h"
#include "clogsetting.h"

class CScreenEditorButtonItem;

class CScreenEditorToolbarItem : public QGraphicsObject
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    CScreenEditorToolbarItem(QGraphicsItem *parent = 0);
    ~CScreenEditorToolbarItem();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    CScreenButtonType getCurrentButtonType();

protected:
//    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
//    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
//    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

private slots:
    void onButtonClicked();

signals:
    void sigButtonClicked(CScreenButtonType type);

private:
    CScreenEditorButtonItem *m_rectButtonItem;
    CScreenEditorButtonItem *m_cancelButtonItem;
    CScreenEditorButtonItem *m_okButtonItem;
    QRectF m_rect;
    CScreenButtonType m_currentButtonType;
    static const int m_buttonWidth = 25;
    static const int m_buttonHeight = 25;
    static const int m_toolbarHeight = 35;
    static const int m_buttonMargin = 8;
};

#endif // _CSCREENEDITORTOOLBARITEM_H_

