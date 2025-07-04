#include "IHttpSessionWare.h"
#include "core/config/IProfileImport.h"
#include "http/detail/IHttpRequestRaw.h"
#include "http/IHttpConstant.h"

$PackageWebCoreBegin

QString IHttpSessionWare::resolveSessionId(const IHttpRequestRaw & raw) const
{
    if(raw.m_cookies.contains(IHttp::SESSION_HEADER)){
        return raw.m_cookies.value(IHttp::SESSION_HEADER).toQString();
    }
    return {};
}

IHttpCookiePart IHttpSessionWare::toCookie(const QString& id, bool isInvalidate) const
{
    static const $Int SESSION_TIME{"/http/session/expiredTime", 30 * 60};
    static const $Bool httpOnly{"/http/session/cookie/httpOnly", false};
    static const $Bool secure{"/http/session/cookie/secure", false};
    static const $QString path{"/http/session/cookie/path", "/"};

    IHttpCookiePart cookie (IHttp::SESSION_HEADER, IString(id), std::chrono::seconds(*SESSION_TIME), *secure, *httpOnly);

    if(isInvalidate){
        cookie.setMaxAge(0);
    }
    if(!(*path).isEmpty()){
        cookie.setPath(IString(*path));
    }
    return cookie;
}

bool IHttpSessionWare::isSessionLoadBeforeInvoke() const
{
    return false;
}

$PackageWebCoreEnd
