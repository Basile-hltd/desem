#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    #include <QRecursiveMutex>
#else
    #include <QMutex>
#endif
#include "critical.h"

volatile unsigned char bInISR = 0;

#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    static QRecursiveMutex mutex;
#else
    static QMutex mutex(QMutex::Recursive);
#endif

void enterCritical()
{
    mutex.lock();
}

void exitCritical()
{
    mutex.unlock();
}
