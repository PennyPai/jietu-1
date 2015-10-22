#include <QPainter>

#include "cscreeneditortoolbaritem.h"
#include "cscreeneditorbuttonitem.h"

#include <QDebug>

CScreenEditorToolbarItem::CScreenEditorToolbarItem(QGraphicsItem *parent)
    :QGraphicsObject(parent)
    ,m_rectButtonItem(NULL)
    ,m_cancelButtonItem(NULL)
    ,m_okButtonItem(NULL)
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

void CScreenEditorToolbarItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__<<"mouseReleaseEvent";
    return QGraphicsObject::mouseReleaseEvent(event);
}

void CScreenEditorToolbarItem::onButtonClicked()
{
    CScreenEditorButtonItem *buttomItem = dynamic_cast<CScreenEditorButtonItem*>(sender());
    if(buttomItem == NULL)
    {
        LOG_WARNING(QString("buttomItem is NULL"));
        return;
    }

    m_currentButtonType = CSCREEN_BUTTON_TYPE_UNDEFINED;
    if(buttomItem == m_rectButtonItem)
    {
        m_currentButtonType = CSCREEN_BUTTON_TYPE_RECT;
    }
    else if(buttomItem == m_cancelButtonItem)
    {
        m_currentButtonType = CSCREEN_BUTTON_TYPE_CANCLE;
    }
    else if(buttomItem == m_okButtonItem)
    {
        m_currentButtonType = CSCREEN_BUTTON_TYPE_OK;
    }

    if(buttomItem != m_rectButtonItem)
    {
        m_rectButtonItem->clearSelected();
    }
    if(buttomItem != m_cancelButtonItem)
    {
        m_cancelButtonItem->clearSelected();
    }
    if(buttomItem != m_okButtonItem)
    {
        m_okButtonItem->clearSelected();
    }
    emit sigButtonClicked(m_currentButtonType);
    LOG_TEST(QString("onButtonClicked"));
}
