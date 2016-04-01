#ifndef CTESTFUNCTION_H
#define CTESTFUNCTION_H

#include <QString>
#include "cscreenshot_global.h"

class QTime;

class CSCREENSHOTSHARED_EXPORT CTestFunction
{
public:
    CTestFunction(const QString &text,bool isTestTime = false);
    ~CTestFunction();

private:
    QString m_text;
    bool m_isTestTime;
    QTime m_time;
};

#endif // CTESTFUNCTION_H
