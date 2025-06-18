#include "IHttpCookiePart.h"
#include "core/util/IConvertUtil.h"
#include "http/IHttpConstant.h"
#include "http/biscuits/IHttpHeader.h"

$PackageWebCoreBegin

const IHttpCookiePart IHttpCookiePart::Empty;

namespace detail
{
    const IString& sameSiteTypeToString(IHttpCookiePart::SameSiteType type);
}

IHttpCookiePart::IHttpCookiePart(IString key, IString value)
    : m_key(std::move(key)), m_value(std::move(value))
{
}

IHttpCookiePart::IHttpCookiePart(IString key, IString value, int maxAge, bool secure, bool httpOnly)
    : m_key(std::move(key)), m_value(std::move(value)), m_maxAge(maxAge), m_secure(secure), m_httpOnly(httpOnly)
{
}

IHttpCookiePart::IHttpCookiePart(IString key, IString value, QDateTime expires, bool secure, bool httpOnly)
    : m_key(std::move(key)), m_value(std::move(value)), m_expires(std::move(expires)), m_secure(secure), m_httpOnly(httpOnly)
{
}

IHttpCookiePart &IHttpCookiePart::setKey(IString key)
{
    this->m_key = std::move(key);
    return *this;
}

IHttpCookiePart &IHttpCookiePart::setValue(IString value)
{
    this->m_value = std::move(value);
    return *this;
}

IHttpCookiePart &IHttpCookiePart::setDomain(IString domain)
{
    this->m_domain = std::move(domain);
    return *this;
}

IHttpCookiePart &IHttpCookiePart::setPath(IString path)
{
    this->m_path = std::move(path);
    return *this;
}

IHttpCookiePart &IHttpCookiePart::setExpires(QDateTime dateTime)
{
    this->m_expires = std::move(dateTime);
    return *this;
}

IHttpCookiePart &IHttpCookiePart::setMaxAge(int maxAge)
{
    this->m_maxAge = maxAge;
    return *this;
}

IHttpCookiePart &IHttpCookiePart::setSecure(bool secure)
{
    this->m_secure = secure;
    return *this;
}

IHttpCookiePart &IHttpCookiePart::setHttpOnly(bool httpOnly)
{
    this->m_httpOnly = httpOnly;
    return *this;
}

IHttpCookiePart &IHttpCookiePart::setSameSite(IHttpCookiePart::SameSiteType sameSite)
{
    this->m_sameSite = sameSite;
    return *this;
}

IStringViewList IHttpCookiePart::toHeaderString() const
{
    static const IString DomainIString = "; Domain=";
    static const IString PathIString = "; Path=";
    static const IString MaxAgeIString = "; Max-Age=";
    static const IString ExpiresIString = "; Expires=";
    static const IString SameSiteIString = "; SameSite=";
    static const IString SecureIString = "; Secure";
    static const IString HttpOnlyIString = "; HttpOnly";

    IStringViewList ret;
    if(m_key.isEmpty() || m_value.isEmpty()){
        return ret;
    }

    ret.push_back(IHttpHeader::SetCookie);
    ret.push_back(IHttp::COMMA_SPACE);
    ret.push_back(m_key);
    ret.push_back(IHttp::EQUAL);
    ret.push_back(m_value);

    if(!m_domain.isEmpty()){
        ret.push_back(DomainIString);
        ret.push_back(m_domain);
    }
    if(!m_path.isEmpty()){
        ret.push_back(PathIString);
        ret.push_back(m_path);
    }
    if(m_maxAge != std::numeric_limits<int>::min()){               // see https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Set-Cookie#browser_compatibility
        m_maxAgeString = std::to_string(m_maxAge);
        ret.push_back(MaxAgeIString);
        ret.push_back(IString(&m_maxAgeString));
    }
    if(m_expires.isValid()){
        m_expiresString = IConvertUtil::toUtcString(m_expires).toStdString();
        ret.push_back(ExpiresIString);
        ret.push_back(IString(&m_expiresString));
    }
    if(m_sameSite != Lax){
        ret.push_back(SameSiteIString);
        ret.push_back(detail::sameSiteTypeToString(m_sameSite));
    }
    if(m_secure){
        ret.push_back(SecureIString);
    }
    if(m_httpOnly){
        ret.push_back(HttpOnlyIString);
    }

    ret.push_back(IHttp::NEW_LINE);
    return ret;
}

bool IHttpCookiePart::isValid()
{
    if(m_key.isEmpty()){
        return false;
    }
    if(m_maxAge != -1 && m_expires.isValid()){
        return false;
    }
    return true;
}

const IString& detail::sameSiteTypeToString(IHttpCookiePart::SameSiteType type)
{
    static const std::vector<IString> strings {
        "Lax",
        "None",
        "Strict"
    };
    return strings[static_cast<int>(type)];
}

$PackageWebCoreEnd
