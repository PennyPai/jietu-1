#include <QPainter>
#include "cscreencolortoolbaritem.h"
#include "cscreeneditorbuttonitem.h"

CScreenColorToolbarItem::CScreenColorToolbarItem(QGraphicsItem *parent)
    :QGraphicsObject(parent)
    ,m_rect(0,0,0,0)
{
    QList<QColor> colorStringList;
    colorStringList.append(QColor("#ff2702"));
    colorStringList.append(QColor("#f95c93"));
    colorStringList.append(QColor("#cd53ca"));
    colorStringList.append(QColor("#0fa4cf"));
    colorStringList.append(QColor("#019e43"));
    colorStringList.append(QColor("#f6cb49"));
    colorStringList.append(QColor("#f96633"));
    colorStringList.append(QColor("#989898"));
    colorStringList.append(QColor("#000000"));
    colorStringList.append(QColor("#ffffff"));
    m_currentColor = colorStringList.first();
    qreal x = 0;
    qreal y = m_margin_top;
    QImage hoverImage(":/screenshot/res/screenshot/colorbuttonbg_hover.png");
    foreach (QColor color, colorStringList)
    {
        QRectF rect(x,y,m_colorItemWidth,m_colorItemHeight);
        CScreenEditorButtonItem *item = createColorItem(hoverImage,color,rect);
        m_colorItemMap.insert(item,color);
        y += m_colorItemHeight;
    }
    y += m_margin_bottom;
    m_rect.setWidth(m_colorItemWidth);
    m_rect.setHeight(y);
}

CScreenColorToolbarItem::~CScreenColorToolbarItem()
{
    //
}

QRectF CScreenColorToolbarItem::boundingRect() const
{
    return m_rect;
}

void CScreenColorToolbarItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing
                            | QPainter::TextAntialiasing
                            | QPainter::SmoothPixmapTransform
                            | QPainter::HighQualityAntialiasing
                            | QPainter::NonCosmeticDefaultPen
                            | QPainter::Qt4CompatiblePainting);
    QPen pen(QColor("#636363"));
    pen.setWidth(1);
    painter->setPen(pen);
    painter->setBrush(QBrush(QColor("#000000")));
    painter->drawRoundedRect(m_rect,5,5);
    painter->restore();
}

QColor CScreenColorToolbarItem::getCurrentColor() const
{
    return m_currentColor;
}

CScreenEditorButtonItem *CScreenColorToolbarItem::createColorItem(const QImage &hoverBackgroundImage, const QColor &color, const QRectF &rect)
{
    QImage imageNormal = createColorItemIcon(color);
    CScreenEditorButtonItem *item = new CScreenEditorButtonItem(imageNormal,imageNormal,rect,this);
    item->setHoverBackground(hoverBackgroundImage);
    item->setPressedBackground(hoverBackgroundImage);
    connect(item,SIGNAL(sigClicked()),this,SLOT(onItemClicked()));
    return item;
}

QImage CScreenColorToolbarItem::createColorItemIcon(const QColor &color)
{
    QPixmap pixmap(m_colorItemWidth,m_colorItemHeight);
    pixmap.fill(QColor(0,0,0,0));
    qreal dw = m_colorItemWidth - m_colorRectWidth;
    qreal dh = m_colorItemHeight - m_colorRectHeight;
    QRectF rect(0.5 * dw,0.5 * dh,m_colorRectWidth,m_colorRectHeight);
    QPen pen(QColor("#ffffff"));
    pen.setWidth(1);
    QPainter painter(&pixmap);
    painter.setPen(pen);
    painter.setBrush(QBrush(color));
    painter.drawRect(rect);
    return pixmap.toImage();
}

void CScreenColorToolbarItem::onItemClicked()
{
    CScreenEditorButtonItem *buttonItem = dynamic_cast<CScreenEditorButtonItem*>(sender());
    if(buttonItem == NULL)
    {
        LOG_WARNING(QString("buttonItem is NULL"));
        return;
    }
    if(m_colorItemMap.contains(buttonItem))
    {
        m_currentColor = m_colorItemMap.value(buttonItem);
        QMapIterator<CScreenEditorButtonItem *,QColor> iter(m_colorItemMap);
        while(iter.hasNext())
        {
            iter.next();
            iter.key()->setSelected(false);
            iter.key()->setIsHover(false);
        }
        this->hide();
        emit sigCurrentColorChanged(m_currentColor);
    }
}
