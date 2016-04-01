#ifndef _CLOGSETTING_H_
#define _CLOGSETTING_H_
#include <QDebug>
#include <QTime>
#include "ctestfunction.h"

#define LOG4QT_DECLARE_QCLASS_LOGGER

//#define LOG_HEAD QString(Q_FUNC_INFO) + QString("[Line:") + QString::number(__LINE__) + QString("]")
#define LOG_HEAD QString("[Line:%1][%2][%3]").arg(QString::number(__LINE__)).arg(Q_FUNC_INFO).arg(QTime::currentTime().toString("hh:mm:ss"))

#define LOG_TEST(testLog) qDebug() << LOG_HEAD << "[TRACE]" << testLog;
#define LOG_TEST_ROOT(testLog) qDebug() << LOG_HEAD << "[TRACE]" << testLog;

#define LOG_DEBUG(debugLog) qDebug() << LOG_HEAD << debugLog;
#define LOG_INFO(infoLog) qWarning() << LOG_HEAD << "[INFO]" << infoLog;
#define LOG_WARNING(warnLog) qWarning() << LOG_HEAD << "[WARN]" << warnLog;
#define LOG_ERROR(errorLog) qCritical() << LOG_HEAD << errorLog;
#define ROOTLOG_ERROR(errorLog) qCritical() << LOG_HEAD << errorLog;

#define LOG_DEBUG_ROOT(debugLog) qDebug() << LOG_HEAD << debugLog;
#define LOG_INFO_ROOT(infoLog) qWarning() << LOG_HEAD << "[INFO]" << infoLog;
#define LOG_WARNING_ROOT(warnLog) qWarning() << LOG_HEAD << "[WARN]" << warnLog;
#define LOG_ERROR_ROOT(errorLog) qCritical() << LOG_HEAD << errorLog;

#define LOG_FUNCTION CTestFunction(QString("[Line:%1][%2]").arg(QString::number(__LINE__)).arg(Q_FUNC_INFO))

#endif // _CLOGSETTING_H_

