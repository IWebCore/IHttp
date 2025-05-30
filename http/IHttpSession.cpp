#include "IHttpSession.h"
#include "core/abort/IGlobalAbort.h"
#include "http/IRequest.h"
#include "http/session/IHttpSessionManager.h"
#include "http/session/IHttpSessionInterface.h"
#include "http/detail/IHttpRequestRaw.h"
#include "http/detail/IHttpRequestImpl.h"

$PackageWebCoreBegin

IHttpSession::IHttpSession(const IRequest &request) : IHttpSession(request.impl())
{
}

IHttpSession::IHttpSession(IHttpRequestImpl& request)
    : IHttpJarUnit(request), m_sessionWare(IHttpSessionManager::instance().getSessionWare())
{
    m_sessionId = m_sessionWare.resolveSessionId(m_impl.m_reqRaw);
    if(!m_sessionId.isEmpty() && m_sessionWare.tryLockId(m_sessionId)){
        return;
    }

    m_sessionId = m_sessionWare.createNewId();
}

const QString &IHttpSession::getId() const
{
    return m_sessionId;
}

void IHttpSession::invalidate()
{
    return m_sessionWare.invalidate(m_sessionId);
}

void IHttpSession::remove(const QString &key)
{
    m_sessionWare.remove(m_sessionId, key);
}

QVariant IHttpSession::getValue(const QString& key, const QVariant& value) const
{
    return m_sessionWare.getValue(m_sessionId, key, value);
}

void IHttpSession::setValue(const QString& key, QVariant value)
{
    m_sessionWare.setValue(m_sessionId, key, value);
}

IHttpCookiePart IHttpSession::toCookie() const
{
    return m_sessionWare.toCookie(m_sessionId);
}

$PackageWebCoreEnd
