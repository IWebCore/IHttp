#include "IResponse.h"

#include "core/abort/IGlobalAbort.h"
#include "http/biscuits/IHttpHeader.h"
#include "http/IRequest.h"
#include "http/detail/IHttpRequestImpl.h"
#include "http/detail/IHttpResponseRaw.h"
#include "http/detail/IHttpRequestRaw.h"
#include "http/IHttpHeaderJar.h"
#include "http/response/IHttpResponseWare.h"

$PackageWebCoreBegin

IResponse::IResponse(IRequest& request) : m_impl(request.impl())
{
}

IHttpCookieJar &IResponse::cookieJar()
{
    return m_impl.m_cookieJar;
}

IHttpSession &IResponse::session()
{
    return m_impl.session();
}

IHttpHeaderJar &IResponse::headerJar()
{
    return m_impl.m_headerJar;
}

//IResponseHeader IResponse::operator[](const IString &header) const
//{
//    return {m_impl.m_respRaw, header};
//}

IResponse &IResponse::setHeader(IStringView key, IStringView value)
{
    m_impl.m_headerJar.addResponseHeader(key, value);
    return *this;
}

IResponse &IResponse::setStatus(IHttpStatus statusCode)
{
    m_impl.m_respRaw.m_status = statusCode;
    return *this;
}

IResponse &IResponse::setStatus(int statusCode)
{
    m_impl.m_respRaw.m_status = IHttpStatus(statusCode);
    return *this;
}

IResponse &IResponse::setMime(IHttpMime mime)
{
    m_impl.m_respRaw.m_mime = IHttpMimeUtil::toString(mime);
    return *this;
}

IResponse &IResponse::setMime(IString mime)
{
    m_impl.m_respRaw.m_mime = std::move(mime);
    return *this;
}

IResponse &IResponse::addCookie(IHttpCookiePart cookiePart)
{
    m_impl.m_respRaw.m_cookies.push_back(std::move(cookiePart));
    return *this;
}

template<>
IResponse &IResponse::setContent<IString&&>(IString && value)
{
    m_impl.m_respRaw.setMime(IHttpMime::TEXT_PLAIN_UTF8);
    m_impl.m_respRaw.setContent(new IHttpResponseContent(std::move(value)));
    return *this;
}

template<>
IResponse &IResponse::setContent<IString>(IString value)
{
    m_impl.m_respRaw.setMime(IHttpMime::TEXT_PLAIN_UTF8);
    m_impl.m_respRaw.setContent(new IHttpResponseContent(std::move(value)));
    return *this;
}

template<>
IResponse &IResponse::setContent<const IString&>(const IString & value)
{
    m_impl.m_respRaw.setMime(IHttpMime::TEXT_PLAIN_UTF8);
    m_impl.m_respRaw.setContent(new IHttpResponseContent(value));
    return *this;
}

template<>
IResponse &IResponse::setContent<const char*>(const char * value)
{
    return setContent(IString(std::string(value)));
}

template<>
IResponse &IResponse::setContent<std::string>(std::string value)
{
    return setContent(IString(std::move(value)));
}

template<>
IResponse &IResponse::setContent<std::string &&>(std::string && value)
{
    return setContent(IString(std::move(value)));
}

template<>
IResponse &IResponse::setContent<const std::string&>(const std::string &value)
{
    return setContent(IString(value));
}

template<>
IResponse &IResponse::setContent<QByteArray>(QByteArray value)
{
    return setContent(IString(std::move(value)));
}

template<>
IResponse &IResponse::setContent<QByteArray&&>(QByteArray&& value)
{
    return setContent(IString(std::move(value)));
}

template<>
IResponse &IResponse::setContent<const QByteArray&>(const QByteArray & value)
{
    return setContent(IString(value));
}

template<>
IResponse &IResponse::setContent<QString>(QString view)
{
    return setContent(IString(view.toStdString()));
}

template<>
IResponse &IResponse::setContent<QString&&>(QString &&view)
{
    return setContent(IString(view.toStdString()));
}

template<>
IResponse &IResponse::setContent<const QString&>(const QString &view)
{
    return setContent(IString(view.toStdString()));
}

template<>
IResponse &IResponse::setContent<IStringView>(IStringView view)
{
    return setContent(IString(view));
}

IResponse &IResponse::setContent(const IHttpResponseWare& ware)
{
    m_impl.setResponseWare(ware);
    return *this;
}

IResponse &IResponse::setContent(const IHttpInvalidWare& unit)
{
    setInvalid(unit);
    return *this;
}

void IResponse::setInvalid(const IHttpInvalidWare& ware)
{
    m_impl.m_isValid = false;
    m_impl.m_respRaw.setContent(ware);
}

IHttpVersion IResponse::version() const
{
    return m_impl.m_reqRaw.m_httpVersion;
}

IStringView IResponse::mime() const
{
    return m_impl.m_respRaw.m_mime.m_view;
}

IHttpStatus IResponse::status() const
{
    return m_impl.m_respRaw.m_status;
}

const IHttpHeader& IResponse::headers() const
{
    return m_impl.m_respRaw.m_headers;
}

$PackageWebCoreEnd
