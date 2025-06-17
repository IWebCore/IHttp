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

IHttpResponseHeader IResponse::operator[](const char *header) const
{
    return IHttpResponseHeader{m_impl.m_respRaw, IString(header)};
}

IHttpResponseHeader IResponse::operator[](const std::string &header) const
{
    return IHttpResponseHeader{m_impl.m_respRaw, IString(header)};
}

IHttpResponseHeader IResponse::operator[](const QString &header) const
{
    return IHttpResponseHeader{m_impl.m_respRaw, IString(header.toStdString())};
}

IHttpResponseHeader IResponse::operator[](const IString &header) const
{
    return IHttpResponseHeader{m_impl.m_respRaw, header};
}

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

IResponse& IResponse::setContent(const char *data)
{
    return setContent(IString(std::string(data)));
}

IResponse &IResponse::setContent(IString &&data)
{
    m_impl.m_respRaw.setMime(IHttpMime::TEXT_PLAIN_UTF8);
    m_impl.m_respRaw.setContent(new IHttpResponseContent(std::move(data)));
    return *this;
}

IResponse &IResponse::setContent(const IString &data)
{
    m_impl.m_respRaw.setMime(IHttpMime::TEXT_PLAIN_UTF8);
    m_impl.m_respRaw.setContent(new IHttpResponseContent(data));
    return *this;
}

IResponse &IResponse::setContent(std::string &&data)
{
    return setContent(IString(std::move(data)));
}

IResponse &IResponse::setContent(const std::string &data)
{
    return setContent(IString(data));
}

IResponse &IResponse::setContent(QByteArray &&data)
{
    return setContent(IString(std::move(data)));
}

IResponse &IResponse::setContent(const QByteArray &data)
{
    return setContent(IString(data));
}

IResponse &IResponse::setContent(const QString &data)
{
    return setContent(IString(data.toStdString()));
}

IResponse &IResponse::setContent(IStringView data)
{
    return setContent(IString(data));
}

IResponse &IResponse::setContent(const IHttpResponseWare &ware)
{
    m_impl.setResponseWare(ware);
    return *this;
}

IResponse &IResponse::setContent(const IHttpInvalidWare &ware)
{
    setInvalid(ware);
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
