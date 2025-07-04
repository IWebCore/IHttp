#pragma once

#include "core/util/IHeaderUtil.h"
#include "http/IHttpJarUnit.h"
#include "http/IHttpCookiePart.h"

$PackageWebCoreBegin

class IRequest;
class IHttpSessionWare;
class IHttpSession : public IHttpJarUnit
{
public:
    IHttpSession(const IRequest& request);
    IHttpSession(IHttpRequestImpl& m_impl);

public:
    static bool isSessionExist(const IRequest& request);

public:
    const QString& getId() const;
    bool isNewSession() const;

    void remove(const QString& key);
    QVariant getValue(const QString& key, const QVariant& variant = {}) const;
    void setValue(const QString& key, QVariant);
    bool contains(const QString& key) const;

    IHttpCookiePart toCookie() const;

    void invalidate();

private:
    bool m_isInvalidated{false};
    bool m_isNewSession{false};
    QString m_sessionId{};
    IHttpSessionWare& m_sessionWare;
};

$PackageWebCoreEnd
