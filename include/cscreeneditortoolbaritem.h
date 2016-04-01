#ifndef _CSCREENEDITORTOOLBARITEM_H_
#define _CSCREENEDITORTOOLBARITEM_H_

#include <QGraphicsObject>
#include "cscreenpublic.h"
#include "cscreenshot_global.h"

class CScreenEditorButtonItem;
class CScreenAttributeToolbarItem;

class CSCREENSHOTSHARED_EXPORT CScreenEditorToolbarItem : public QGraphicsObject
{
    Q_OBJECT

public:
    CScreenEditorToolbarItem(QGraphicsItem *parent = 0);
    ~CScreenEditorToolbarItem();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    CScreenButtonType getCurrentButtonType();
    int getLineWidth() const;
    QColor getColor() const;

private:
    void updateAttributeToolbar(const qreal centerX);

private slots:
    void onButtonClicked();

signals:
    void sigButtonClicked(CScreenButtonType type);

private:
    CScreenEditorButtonItem *m_rectButtonItem;
    CScreenEditorButtonItem *m_cancelButtonItem;
    CScreenEditorButtonItem *m_okButtonItem;
    CScreenAttributeToolbarItem *m_attributeToolbarItem;
    QRectF m_rect;
    CScreenButtonType m_currentButtonType;
    static const int m_buttonWidth = 25;
    static const int m_buttonHeight = 25;
    static const int m_toolbarHeight = 35;
    static const int m_buttonMargin = 8;
    static const int m_marginAttributeToolbarWidthBottom = 10;
};

#endif // _CSCREENEDITORTOOLBARITEM_H_

