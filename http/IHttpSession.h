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
    const QString& getId() const;

    void remove(const QString& key);
    QVariant getValue(const QString& key, const QVariant& variant = {}) const;
    void setValue(const QString& key, QVariant);

    IHttpCookiePart toCookie() const;

    void invalidate();

private:
    IHttpSessionWare& m_sessionWare;
    QString m_sessionId{};
};

$PackageWebCoreEnd
