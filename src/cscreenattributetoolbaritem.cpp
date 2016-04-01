#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "cscreenattributetoolbaritem.h"
#include "cscreeneditorbuttonitem.h"
#include "cscreencolortoolbaritem.h"

CScreenAttributeToolbarItem::CScreenAttributeToolbarItem(QGraphicsItem *parent)
    :QGraphicsObject(parent)
    , m_lineWidthSmallButtonItem(NULL)
    , m_lineWidthMiddleButtonItem(NULL)
    , m_lineWidthLargeButtonItem(NULL)
    , m_colorButtonItem(NULL)
    , m_rect(0,0,0,0)
    , m_arrowDirection(CSCREEN_ARROW_DIRECTION_UP)
    , m_lineWidth(m_lineWidthSmall)
    , m_arrawDx(0)
{
    m_colorToolbarItem = new CScreenColorToolbarItem(this);
    m_colorToolbarItem->setVisible(false);
    connect(m_colorToolbarItem,SIGNAL(sigCurrentColorChanged(QColor)),
            this,SLOT(onCurrentColorChanged(QColor)));
    m_color = m_colorToolbarItem->getCurrentColor();
    m_colorToolbarItem->setZValue(this->zValue() + 1);

    QImage backgroundImage(":/screenshot/res/screenshot/linewidthbg_pressed.png");
    m_lineWidthSmallButtonItem = createLineWidthItem(QImage(":/screenshot/res/screenshot/linewidth_small.png"),backgroundImage);
    m_lineWidthMiddleButtonItem = createLineWidthItem(QImage(":/screenshot/res/screenshot/linewidth_middle.png"),backgroundImage);
    m_lineWidthLargeButtonItem = createLineWidthItem(QImage(":/screenshot/res/screenshot/linewidth_large.png"),backgroundImage);
    QImage colorImage = createColorItemBackground(m_color);
    m_colorButtonItem = createLineWidthItem(colorImage,QImage());
    m_colorButtonItem->setNormalBackground(QImage(":/screenshot/res/screenshot/colorbuttonbg_normal.png"));
    m_colorButtonItem->setHoverBackground(QImage(":/screenshot/res/screenshot/colorbuttonbg_normal.png"));
    m_lineWidthSmallButtonItem->setSelected(true);

    updateLayout();
}

CScreenAttributeToolbarItem::~CScreenAttributeToolbarItem()
{

}

QRectF CScreenAttributeToolbarItem::boundingRect() const
{
    return m_rect;
}

void CScreenAttributeToolbarItem::paint(QPainter *painter,
                                        const QStyleOptionGraphicsItem *option,
                                        QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing
                            | QPainter::TextAntialiasing
                            | QPainter::SmoothPixmapTransform
                            | QPainter::HighQualityAntialiasing
                            | QPainter::NonCosmeticDefaultPen
                            | QPainter::Qt4CompatiblePainting);
    qreal y = 0;
    if(m_arrowDirection == CSCREEN_ARROW_DIRECTION_UP)
    {
        y = m_triangleHeight;
    }
    QRectF rect(0,y,m_rect.width(),m_rect.height() - m_triangleHeight);
    QPen pen(QColor("#636363"));
    pen.setWidth(1);
    QBrush brush(QColor("#000000"));
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawRoundedRect(rect,5,5);
    {//draw triangle

        qreal centerX = m_rect.center().x();
        qreal leftX = centerX + m_arrawDx - m_triangleWidth * 0.5;
        qreal middleX = centerX + m_arrawDx;
        qreal rightX = centerX + m_arrawDx + m_triangleWidth * 0.5;

        qreal leftY = m_triangleHeight;
        qreal middleY = 0;
        qreal dy = 0.5;
        if(m_arrowDirection == CSCREEN_ARROW_DIRECTION_DOWN)
        {
            leftY = rect.height();
            middleY = leftY + m_triangleHeight;
            dy = -0.5;
        }
        qreal rightY = leftY;
        {//draw polygon
        QPolygonF polygon;
        QPointF leftPos(leftX,leftY + dy);
        QPointF middlePos(middleX,middleY);
        QPointF rightPos(rightX,rightY + dy);
        polygon.append(leftPos);
        polygon.append(middlePos);
        polygon.append(rightPos);
        polygon.append(leftPos);

            QPen pen(Qt::NoPen);
            painter->setPen(pen);
            painter->drawPolygon(polygon);
        }
        {//draw edge
            painter->setPen(pen);
            QPointF leftPos(leftX,leftY);
            QPointF middlePos(middleX,middleY);
            QPointF rightPos(rightX,rightY);

            painter->drawLine(QLineF(leftPos,middlePos));
            painter->drawLine(QLineF(rightPos,middlePos));
        }
    }
    painter->restore();
}

void CScreenAttributeToolbarItem::setArrowDirectionPosition(CScreenArrowDirection direction, const qreal dx)
{
    if(m_arrowDirection == direction && m_arrawDx == dx)
    {
        return;
    }
    m_arrawDx = dx;
    if(m_arrowDirection != direction)
    {
        m_arrowDirection = direction;
        updateLayout();
    }
    else
    {
        update();
    }
}

int CScreenAttributeToolbarItem::getLineWidth() const
{
    return m_lineWidth;
}

QColor CScreenAttributeToolbarItem::getColor() const
{
    return m_color;
}

CScreenEditorButtonItem *CScreenAttributeToolbarItem::createLineWidthItem(const QImage &icon, const QImage &pressedBackground)
{
    CScreenEditorButtonItem *item = new CScreenEditorButtonItem(icon,icon,QRectF(0,0,0,0),this);
    item->setPressedBackground(pressedBackground);
    item->setHoverBackground(QImage());
    connect(item,SIGNAL(sigClicked()),this,SLOT(onItemClicked()));
    return item;
}

void CScreenAttributeToolbarItem::updateLayout()
{
    qreal y = m_marginTop;
    if(m_arrowDirection == CSCREEN_ARROW_DIRECTION_UP)
    {
        y += m_triangleHeight;
    }
    qreal x = m_marginLeft;
    m_lineWidthSmallButtonItem->setRect(QRectF(x,y,m_lineItemWidth,m_lineItemHeight));
    x += m_marginLineWidthItem + m_lineItemWidth;
    m_lineWidthMiddleButtonItem->setRect(QRectF(x,y,m_lineItemWidth,m_lineItemHeight));
    x += m_marginLineWidthItem + m_lineItemWidth;
    m_lineWidthLargeButtonItem->setRect(QRectF(x,y,m_lineItemWidth,m_lineItemHeight));
    x += m_marginLineWidthWithColorItem + m_lineItemWidth;
    m_colorButtonItem->setRect(QRectF(x,y,m_colorItemWidth,m_colorItemHeight));
    m_rect.setWidth(x + m_colorItemWidth + m_marginRight);
    m_rect.setHeight(m_marginTop + m_lineItemHeight + m_triangleHeight + m_marginBottom);
    update();
}

QImage CScreenAttributeToolbarItem::createColorItemBackground(const QColor &color)
{
    QPixmap pixmap(m_colorItemWidth,m_colorItemHeight);
    pixmap.fill(QColor(0,0,0,0));
    QPainter painter(&pixmap);
    QPen pen(QColor("#ffffff"));
    pen.setWidth(1);
    painter.setPen(pen);
    painter.setBrush(QBrush(color));
    QRectF rect(9,(m_colorItemHeight - 12) * 0.5,34,12);
    painter.drawRect(rect);
    return pixmap.toImage();
}

void CScreenAttributeToolbarItem::onItemClicked()
{
    CScreenEditorButtonItem *buttonItem = dynamic_cast<CScreenEditorButtonItem*>(sender());
    if(buttonItem == NULL)
    {
        C_SCREENSHOTSHARED_LOG(QString("buttonItem is NULL"));
        return;
    }
    m_colorToolbarItem->setVisible(false);

    if(buttonItem == m_lineWidthSmallButtonItem)
    {
        m_lineWidth = m_lineWidthSmall;
        //        m_lineWidthSmallItem->setSelected(false);
        m_lineWidthMiddleButtonItem->setSelected(false);
        m_lineWidthLargeButtonItem->setSelected(false);
    }
    else if(buttonItem == m_lineWidthMiddleButtonItem)
    {
        m_lineWidth = m_lineWidthMiddle;
        m_lineWidthSmallButtonItem->setSelected(false);
        //        m_lineWidthMiddleItem->setSelected(false);
        m_lineWidthLargeButtonItem->setSelected(false);
    }
    else if(buttonItem == m_lineWidthLargeButtonItem)
    {
        m_lineWidth = m_lineWidthLarge;
        m_lineWidthSmallButtonItem->setSelected(false);
        m_lineWidthMiddleButtonItem->setSelected(false);
        //        m_lineWidthLargeItem->setSelected(false);
    }
    else if(buttonItem == m_colorButtonItem)
    {
        QGraphicsScene *scene = this->scene();
        if(scene == NULL)
        {
            return;
        }
        QList<QGraphicsView *> views = scene->views();
        if(views.isEmpty())
        {
            return;
        }
        qreal veiwHeight = views.first()->height();
        qreal viewWidth = views.first()->width();
        QPointF maxPoint = this->mapFromScene(QPointF(viewWidth,veiwHeight));

        m_colorButtonItem->setSelected(false);
        qreal x = m_colorButtonItem->boundingRect().x();
        qreal y = 0;
        qreal bottomY = y + m_colorToolbarItem->boundingRect().height() + m_marginBottomColorToolbarToView;
        if(bottomY > maxPoint.y())
        {
            y = maxPoint.y() - m_marginBottomColorToolbarToView - m_colorToolbarItem->boundingRect().height();
        }

        m_colorToolbarItem->setPos(x,y);
        m_colorToolbarItem->setVisible(true);
    }
}

void CScreenAttributeToolbarItem::onCurrentColorChanged(const QColor &color)
{
    if(m_color == color)
    {
        return;
    }
    m_color = color;
    QImage colorImage = createColorItemBackground(m_color);
    m_colorButtonItem->setNormalIcon(colorImage);
}
