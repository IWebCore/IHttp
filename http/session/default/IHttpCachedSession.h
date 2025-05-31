#pragma once

#include "core/util/IHeaderUtil.h"
#include "core/base/IHandle.h"
#include <shared_mutex>
#include "http/session/IHttpSessionInterface.h"

$PackageWebCoreBegin

struct IHttpCachedSessionData;
class IHttpCachedSession : public IHttpSessionInterface<IHttpCachedSession>
{
public:
    IHttpCachedSession();

public:
    virtual double $order() const final;

public:
    virtual QString createNewId() final;
    virtual bool tryLockId(const QString& id) const final;

    virtual void remove(const QString& id, const QString& key);
    virtual QVariant getValue(const QString& id, const QString& key, const QVariant& defaultValue={}) const;
    virtual void setValue(const QString& id, const QString& key, const QVariant& value) final;

    virtual void invalidate(const QString& id) final;

public:
    virtual void startTimer() final;
    virtual void stopTimer() final;

private:
    void reduceSession();

private:
    mutable std::shared_mutex m_mutex;
    QHash<QString, IHttpCachedSessionData*> m_store;
    IHandle m_timerId{};
};

$PackageWebCoreEnd
