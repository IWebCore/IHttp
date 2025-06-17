#pragma once

#include "core/util/IHeaderUtil.h"

$PackageWebCoreBegin

class IHttpCookiePart
{
public:
    enum SameSiteType{
        Lax,
        None,
        Strict
    };

public:
    IHttpCookiePart() = default;
    IHttpCookiePart(IString m_key, IString m_value);

    template<typename T>
    explicit IHttpCookiePart(IString key, IString value, std::chrono::duration<T> duration, bool secure=false, bool httpOnly=false);
    explicit IHttpCookiePart(IString key, IString value, int maxAge, bool secure=false, bool httpOnly=false);
    explicit IHttpCookiePart(IString key, IString value, QDateTime expires, bool secure=false, bool httpOnly=false);

public:
    IHttpCookiePart& setKey(IString m_key);
    IHttpCookiePart& setValue(IString m_value);
    IHttpCookiePart& setDomain(IString m_domain);
    IHttpCookiePart& setPath(IString m_path);
    IHttpCookiePart& setExpires(QDateTime dateTime);
    IHttpCookiePart& setMaxAge(int m_maxAge);
    IHttpCookiePart& setSecure(bool m_secure);
    IHttpCookiePart& setHttpOnly(bool m_httpOnly);
    IHttpCookiePart& setSameSite(SameSiteType m_sameSite);

    IStringViewList toHeaderString() const;

    bool isValid();

public:
    IString m_key;
    IString m_value;
    IString m_domain;
    IString m_path;

    QDateTime m_expires {};
    int m_maxAge {std::numeric_limits<int>::min()};

    bool m_secure {false};
    bool m_httpOnly {false};
    SameSiteType m_sameSite{Lax};

private:
    mutable std::string m_maxAgeString;
    mutable std::string m_expiresString;

public:
    static const IHttpCookiePart Empty;
};

template<typename T>
IHttpCookiePart::IHttpCookiePart(IString key, IString value, std::chrono::duration<T> duration, bool secure, bool httpOnly)
    : IHttpCookiePart(std::move(key), std::move(value), std::chrono::duration_cast<std::chrono::seconds>(duration).count(), secure, httpOnly)
{
}

$PackageWebCoreEnd
