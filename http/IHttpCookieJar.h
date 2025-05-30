#pragma once

#include "core/util/IHeaderUtil.h"
#include "http/IHttpCookiePart.h"
#include "http/IHttpJarUnit.h"
#include <chrono>

$PackageWebCoreBegin

class IHttpCookieJar : public IHttpJarUnit
{
public:
    using IHttpJarUnit::IHttpJarUnit;

public:
    const QMultiHash<IStringView, IStringView>& requestCookies() const;
    IHttpCookiePart getRequestCookie(IString key) const;
    QList<IHttpCookiePart> getRequestCookies(const IString& key) const;

    IStringViewList requestCookieKeys() const;

    bool containRequestCookieKey(const IString& key) const;

    void deleteRequestCookies(const IString& key);

    // response
    std::list<IHttpCookiePart>& responseCookies();
    const std::list<IHttpCookiePart>& responseCookies() const;
    IHttpCookiePart getResponseCookie(const IString& key) const;
    QList<IString> responseCookieKeys() const;
    bool containResponseCookieKey(const IString& key) const;
    void deleteResponseCookie(const IString& key);

    template<typename T>        // in latter c++14, you can pass 1h, 24h type.
    void addResponseCookie(IString key, IString value, std::chrono::duration<T> duration, bool secure=false, bool httpOnly=false);
    void addResponseCookie(IHttpCookiePart cookiePart);
    void addResponseCookie(IString key, IString value);
    void addResponseCookie(IString key, IString value, int maxAge, bool secure=false, bool httpOnly=false);
    void addResponseCookie(IString key, IString value, QDateTime expires, bool secure=false, bool httpOnly=false);
};

template<typename T>
void IHttpCookieJar::addResponseCookie(IString key, IString value, std::chrono::duration<T> duration, bool secure, bool httpOnly)
{
    IHttpCookiePart part(std::move(key), std::move(value), duration, secure, httpOnly);
    addResponseCookie(std::move(part));
}

$PackageWebCoreEnd
