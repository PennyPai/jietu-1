#ifndef _CSCREENTOOLTIPITEM_H_
#define _CSCREENTOOLTIPITEM_H_

#include <QGraphicsObject>
#include "cscreenshot_global.h"

class CSCREENSHOTSHARED_EXPORT CScreenTooltipItem : public QGraphicsObject
{
    Q_OBJECT

public:
    CScreenTooltipItem(QGraphicsItem *parent = 0);
    ~CScreenTooltipItem();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    void setText(const QString &text);

private:
    QRectF m_rect;
    QString m_text;
    QColor m_colorBackgroud;
    QColor m_colorText;
    static const int m_textPointSize = 12;
    static const int m_rectRadius = 3;
};

#endif // _CSCREENTOOLTIPITEM_H_
