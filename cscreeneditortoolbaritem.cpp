#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "cscreeneditortoolbaritem.h"
#include "cscreeneditorbuttonitem.h"
#include "cscreenattributetoolbaritem.h"

#include <QDebug>

CScreenEditorToolbarItem::CScreenEditorToolbarItem(QGraphicsItem *parent)
    :QGraphicsObject(parent)
    ,m_rectButtonItem(NULL)
    ,m_cancelButtonItem(NULL)
    ,m_okButtonItem(NULL)
    ,m_attributeToolbarItem(NULL)
    ,m_rect(0,0,300,m_toolbarHeight)
    ,m_currentButtonType(CSCREEN_BUTTON_TYPE_UNDEFINED)
{
    qreal x = m_buttonMargin;
    qreal y = 0.5 * (m_toolbarHeight - m_buttonHeight);

    {
        QImage imageNormal(":/screenshot/res/screenshot/rect_icon_normal.png");
        QImage imagePressed(":/screenshot/res/screenshot/rect_icon_pressed.png");
        QRectF rect(x,y,m_buttonWidth,m_buttonHeight);
        m_rectButtonItem = new CScreenEditorButtonItem(imageNormal,imagePressed,rect,this);
        connect(m_rectButtonItem,SIGNAL(sigClicked()),this,SLOT(onButtonClicked()));
    }
    x += m_buttonMargin + m_buttonWidth;
    {
        QImage imageNormal(":/screenshot/res/screenshot/cancel_icon_normal.png");
        QImage imagePressed(":/screenshot/res/screenshot/cancel_icon_normal.png");
        QRectF rect(x,y,m_buttonWidth,m_buttonHeight);
        m_cancelButtonItem = new CScreenEditorButtonItem(imageNormal,imagePressed,rect,this);
        connect(m_cancelButtonItem,SIGNAL(sigClicked()),this,SLOT(onButtonClicked()));
    }
    x += m_buttonMargin + m_buttonWidth;
    {
        QImage imageNormal(":/screenshot/res/screenshot/confirm_icon_normal.png");
        QImage imagePressed(":/screenshot/res/screenshot/confirm_icon_normal.png");
        QRectF rect(x,y,m_buttonWidth,m_buttonHeight);
        m_okButtonItem = new CScreenEditorButtonItem(imageNormal,imagePressed,rect,this);
        connect(m_okButtonItem,SIGNAL(sigClicked()),this,SLOT(onButtonClicked()));
    }
    x += m_buttonMargin + m_buttonWidth;
    m_rect.setWidth(x);
    {
        m_attributeToolbarItem = new CScreenAttributeToolbarItem(this);
        m_attributeToolbarItem->setZValue(this->zValue() + 1);
        m_attributeToolbarItem->setPos(0,this->boundingRect().height());
        m_attributeToolbarItem->setVisible(false);
    }
}

CScreenEditorToolbarItem::~CScreenEditorToolbarItem()
{

}

QRectF CScreenEditorToolbarItem::boundingRect() const
{
    return m_rect;
}

void CScreenEditorToolbarItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawImage(m_rect,QImage(":/screenshot/res/screenshot/toolbarbg.png"));
}

CScreenButtonType CScreenEditorToolbarItem::getCurrentButtonType()
{
    return m_currentButtonType;
}

int CScreenEditorToolbarItem::getLineWidth() const
{
    return m_attributeToolbarItem->getLineWidth();
}

QColor CScreenEditorToolbarItem::getColor() const
{
    return m_attributeToolbarItem->getColor();
}

void CScreenEditorToolbarItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    LOG_TEST("mouseReleaseEvent");
    return QGraphicsObject::mouseReleaseEvent(event);
}

void CScreenEditorToolbarItem::updateAttributeToolbar(const qreal centerX)
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
    LOG_TEST(QString("pos(%1,%2),viewWidth %3,veiwHeight %4,maxPoint(%5,%6)")
             .arg(this->pos().x()).arg(this->pos().y())
             .arg(viewWidth).arg(veiwHeight)
             .arg(maxPoint.x()).arg(maxPoint.y()));

    qreal dx = centerX - m_attributeToolbarItem->boundingRect().width() / 2 ;
    qreal x = dx;
    if(dx < C_SCREEN_ZERO)
    {
        x = 0;
    }
    if(x + m_attributeToolbarItem->boundingRect().width() > maxPoint.x())
    {
        x = maxPoint.x() - m_attributeToolbarItem->boundingRect().width();
    }
    dx =  centerX - (x + 0.5 * m_attributeToolbarItem->boundingRect().width());

    qreal overlapHeight = 0.5 * (m_toolbarHeight - m_buttonWidth);
    qreal y = m_rect.height() - overlapHeight;
    qreal bottomY = y + m_attributeToolbarItem->boundingRect().height() + m_marginAttributeToolbarWidthBottom;

    CScreenArrowDirection arrowDirection = CSCREEN_ARROW_DIRECTION_UP;
    if(bottomY > maxPoint.y())
    {
        y = overlapHeight - m_attributeToolbarItem->boundingRect().height();
        arrowDirection = CSCREEN_ARROW_DIRECTION_DOWN;
    }
    m_attributeToolbarItem->setArrowDirectionPosition(arrowDirection,dx);
    m_attributeToolbarItem->setPos(x,y);
    m_attributeToolbarItem->setVisible(true);
}

void CScreenEditorToolbarItem::onButtonClicked()
{
    CScreenEditorButtonItem *buttonItem = dynamic_cast<CScreenEditorButtonItem*>(sender());
    if(buttonItem == NULL)
    {
        LOG_WARNING(QString("buttonItem is NULL"));
        return;
    }

    m_currentButtonType = CSCREEN_BUTTON_TYPE_UNDEFINED;
    if(buttonItem == m_rectButtonItem)
    {
        m_currentButtonType = CSCREEN_BUTTON_TYPE_RECT;
        updateAttributeToolbar(buttonItem->boundingRect().center().x());
    }
    else if(buttonItem == m_cancelButtonItem)
    {
        m_currentButtonType = CSCREEN_BUTTON_TYPE_CANCLE;
    }
    else if(buttonItem == m_okButtonItem)
    {
        m_currentButtonType = CSCREEN_BUTTON_TYPE_OK;
    }

    if(buttonItem != m_rectButtonItem)
    {
        m_rectButtonItem->setSelected();
    }
    if(buttonItem != m_cancelButtonItem)
    {
        m_cancelButtonItem->setSelected();
    }
    if(buttonItem != m_okButtonItem)
    {
        m_okButtonItem->setSelected();
    }
    emit sigButtonClicked(m_currentButtonType);
    LOG_TEST(QString("onButtonClicked"));
}
