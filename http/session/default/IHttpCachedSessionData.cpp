#include "IHttpCachedSessionData.h"

#include "core/application/IApplication.h"
#include "core/config/IProfileImport.h"

$PackageWebCoreBegin

IHttpCachedSessionData::IHttpCachedSessionData(const QString& key)
    : m_key(key), m_lastVisitTime(IApplication::time())
{
}

bool IHttpCachedSessionData::isExpired() const
{
    static $LongLong time{"/http/session/expiredTime", 30*60};
    static long long expiredTime = *time * 1000 * 1000 * 1000;
    return (IApplication::time() - m_lastVisitTime) > expiredTime;
}

bool IHttpCachedSessionData::isClearable(std::int64_t current) const
{
    static $LongLong time{"/http/session/clearTime", 35 * 60};
    static long long clearTime = *time * 1000 * 1000 * 1000;
    return (current - m_lastVisitTime) > clearTime;
}

void IHttpCachedSessionData::remove(const QString &key)
{
    m_data.remove(key);
}

QVariant IHttpCachedSessionData::value(const QString &name, const QVariant &val) const
{
    return m_data.value(name, val);
}

void IHttpCachedSessionData::setValue(const QString &name, const QVariant &value)
{
    m_data[name] = value;
}

void IHttpCachedSessionData::updateLastVisitTime()
{
    m_lastVisitTime = IApplication::time();
}

$PackageWebCoreEnd
