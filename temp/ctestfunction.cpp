#include <QTime>
#include "ctestfunction.h"

CTestFunction::CTestFunction(const QString &text, bool isTestTime)
    :m_text(text)
    ,m_isTestTime(m_isTestTime)
    ,m_time(QTime::currentTime())
{
    C_SCREENSHOTSHARED_LOG(QString("%1 start").arg(m_text));
}

CTestFunction::~CTestFunction()
{
    QString timeText;
    if(m_isTestTime)
    {
        timeText = QString("time %1ms").arg(QTime::currentTime().msecsTo(m_time));
    }
    C_SCREENSHOTSHARED_LOG(QString("%1 %2").arg(m_text).arg(timeText));
}
