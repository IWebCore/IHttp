#include "IHttpCachedSession.h"
#include "core/application/IApplication.h"
#include "core/application/IAsioContext.h"
#include "core/config/IProfileImport.h"
#include "http/session/default/IHttpCachedSessionData.h"

$PackageWebCoreBegin

IHttpCachedSession::IHttpCachedSession()
{
    startTimer();
}

double IHttpCachedSession::$order() const
{
    return 1;
}

QString IHttpCachedSession::createNewId()
{
    auto id = QString::number(IApplication::time());
    {
        std::unique_lock<std::shared_mutex> lock(m_mutex);
        m_store[id] = new IHttpCachedSessionData(id);
    }
    return id;
}

bool IHttpCachedSession::tryLockId(const QString &id) const
{
    std::shared_lock<std::shared_mutex> lock(m_mutex);
    if(m_store.contains(id)){
        auto ptr = m_store[id];
        if(!ptr->isExpired()){
            ptr->updateLastVisitTime();
            return true;
        }
    }
    return false;
}

void IHttpCachedSession::remove(const QString& id, const QString &key)
{
    std::shared_lock<std::shared_mutex> lock(m_mutex);
    if(m_store.contains(id)){
        return m_store[id]->remove(key);
    }
}

QVariant IHttpCachedSession::getValue(const QString &id, const QString &key, const QVariant& defaultValue) const
{
    std::shared_lock<std::shared_mutex> lock(m_mutex);
    if(m_store.contains(id)){
        return m_store[id]->value(key, defaultValue);
    }
    return defaultValue;
}

void IHttpCachedSession::setValue(const QString &id, const QString &key, const QVariant &value)
{
    std::shared_lock<std::shared_mutex> lock(m_mutex);
    if(m_store.contains(id)){
        m_store[id]->setValue(key, value);
    }
}

void IHttpCachedSession::invalidate(const QString &id)
{
    std::shared_lock<std::shared_mutex> lock(m_mutex);
    if(m_store.contains(id)){
        m_store[id]->m_lastVisitTime = 0;
    }
}

void IHttpCachedSession::startTimer()
{
    static $Int time{"/http/session/clearTime", 35 * 60};
    m_timerId = IAsioContext::startTimer(std::chrono::seconds(*time), [&](){
        reduceSession();
    });
}

void IHttpCachedSession::stopTimer()
{
    if(m_timerId){
        IAsioContext::stopTimer(m_timerId);
        m_timerId = 0;
    }
}

void IHttpCachedSession::reduceSession()
{
    std::unique_lock<std::shared_mutex> lock(m_mutex);
    auto time = IApplication::time();
    for(auto it = m_store.begin(); it != m_store.end();){
        if(it.value()->isClearable(time)){
            it = m_store.erase(it);
        }else{
            it++;
        }
    }
}

$PackageWebCoreEnd
