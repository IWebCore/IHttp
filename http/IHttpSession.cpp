#include "IHttpSession.h"
#include "core/config/IProfileImport.h"
#include "http/IRequest.h"
#include "http/IHttpConstant.h"
#include "http/session/IHttpSessionManager.h"
#include "http/session/IHttpSessionWare.h"
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

    m_isNewSession = true;
    m_sessionId = m_sessionWare.createNewId();
}

bool IHttpSession::isSessionEnabled()
{
    static $Bool enabled{"/http/session/enabled", false};
    return *enabled;
}

bool IHttpSession::isSessionExist(const IRequest &request)
{
    auto value = request.impl().m_reqRaw.m_cookies.value(IHttp::SESSION_HEADER, {});
    if(value.length() == 0){
        return false;
    }
    auto& ware = IHttpSessionManager::instance().getSessionWare();
    return ware.tryLockId(value.toQString());
}

const QString &IHttpSession::getId() const
{
    return m_sessionId;
}

bool IHttpSession::isNewSession() const
{
    return m_isNewSession;
}

void IHttpSession::invalidate()
{
    m_isInvalidated = true;
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

bool IHttpSession::contains(const QString &key) const
{
    return m_sessionWare.contains(m_sessionId, key);
}

IHttpCookiePart IHttpSession::toCookie() const
{
    return m_sessionWare.toCookie(m_sessionId, m_isInvalidated);
}

$PackageWebCoreEnd
