#pragma once

#include "core/util/IHeaderUtil.h"
#include "http/IHttpCookiePart.h"

$PackageWebCoreBegin

class IHttpRequestRaw;
class IHttpSessionWare
{
public:
    IHttpSessionWare() = default;

public:
    virtual QString createNewId() = 0;
    virtual QString resolveSessionId(const IHttpRequestRaw&) const;

    virtual bool tryLockId(const QString& id) const = 0;

    virtual void remove(const QString& id, const QString& key) = 0;
    virtual QVariant getValue(const QString& id, const QString& key, const QVariant& defaultValue = {}) const = 0;
    virtual void setValue(const QString& id, const QString& key, const QVariant& value) = 0;

    virtual void invalidate(const QString& id) = 0;

    virtual IHttpCookiePart toCookie(const QString& id) const;

public:
    virtual void startTimer() = 0;
    virtual void stopTimer() = 0;
    virtual bool isSessionLoadBeforeInvoke() const;
};

$PackageWebCoreEnd
