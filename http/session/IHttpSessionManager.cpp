#include "IHttpSessionManager.h"
#include "IHttpSessionInterface.h"

$PackageWebCoreBegin

void IHttpSessionManager::registerSessionWare(IHttpSessionWare *sessionWare)
{
    if(m_sessionWare){
        m_sessionWare->stopTimer();
    }
    m_sessionWare = sessionWare;
    m_sessionWare->startTimer();
}

IHttpSessionWare& IHttpSessionManager::getSessionWare()
{
    return *m_sessionWare;
}

$PackageWebCoreEnd

