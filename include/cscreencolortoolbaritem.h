#ifndef _CSCREENCOLORTOOLBARITEM_H_
#define _CSCREENCOLORTOOLBARITEM_H_

#include <QGraphicsObject>
#include "cscreenshot_global.h"
#include "cscreenpublic.h"

class CScreenEditorButtonItem;

class CSCREENSHOTSHARED_EXPORT CScreenColorToolbarItem : public QGraphicsObject
{
    Q_OBJECT

public:
    CScreenColorToolbarItem(QGraphicsItem *parent = 0);
    ~CScreenColorToolbarItem();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    QColor getCurrentColor() const;

private:
    CScreenEditorButtonItem *createColorItem(const QImage &hoverBackgroundImage,const QColor &color,const QRectF &rect);
    QImage createColorItemIcon(const QColor &color);

private slots:
    void onItemClicked();

signals:
    void sigCurrentColorChanged(const QColor &color);

private:
    QRectF m_rect;
    QMap<CScreenEditorButtonItem *,QColor> m_colorItemMap;
    QColor m_currentColor;
    static const int m_margin_top = 5;
    static const int m_margin_bottom = 5;
    static const int m_colorRectWidth = 42;
    static const int m_colorRectHeight = 16;
    static const int m_colorItemWidth = 60;
    static const int m_colorItemHeight = 20;
};

#endif // _CSCREENCOLORTOOLBARITEM_H_
