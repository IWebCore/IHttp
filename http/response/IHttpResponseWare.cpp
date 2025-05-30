#include "IHttpResponseWare.h"

#include "core/abort/IGlobalAbort.h"
#include "http/detail/IHttpResponseRaw.h"

$PackageWebCoreBegin

IHttpResponseWare::IHttpResponseWare() : m_raw(new IHttpResponseRaw())
{
}

IHttpResponseWare::~IHttpResponseWare()
{
    delete m_raw;
    m_raw = nullptr;
}

IHttpResponseWare::IHttpResponseWare(IHttpResponseWare && rhs)
{
    std::swap(this->m_raw, rhs.m_raw);
}

IHttpResponseWare::IHttpResponseWare(const IHttpResponseWare &rhs)
    : m_raw(new IHttpResponseRaw(*rhs.m_raw))
{
}

IHttpResponseWare &IHttpResponseWare::operator =(const IHttpResponseWare & rhs)
{
    delete this->m_raw;
    this->m_raw = new IHttpResponseRaw(*rhs.m_raw);
    return *this;
}

IHttpResponseWare &IHttpResponseWare::operator = (IHttpResponseWare && rhs)
{
    std::swap(this->m_raw, rhs.m_raw);
    return *this;
}

//IResponseHeader IResponseWare::operator[](const IString &header)
//{
//    return IResponseHeader(*m_raw, header);
//}

const IString& IHttpResponseWare::mime() const
{
    return m_raw->m_mime;
}

void IHttpResponseWare::setMime(IHttpMime mime)
{
    m_raw->setMime(mime);
}

IHttpStatus IHttpResponseWare::status() const
{
    return m_raw->m_status;
}

void IHttpResponseWare::setStatus(IHttpStatus statusCode)
{
    m_raw->m_status = statusCode;
}

const IHttpHeader& IHttpResponseWare::headers() const
{
    return m_raw->m_headers;
}

IHttpHeader &IHttpResponseWare::headers()
{
    return m_raw->m_headers;
}

void IHttpResponseWare::setHeader(IStringView key, IStringView value)
{
    m_raw->m_headers.insert(key, value);
}

void IHttpResponseWare::setCookie(IHttpCookiePart && cookie)
{
    m_raw->setCookie(std::move(cookie));
}

void IHttpResponseWare::setCookie(const IHttpCookiePart & cookie)
{
    m_raw->setCookie(cookie);
}

void IHttpResponseWare::setCookie(const IString &key, const IString &value)
{
    m_raw->setCookie(key, value);
}

std::string IHttpResponseWare::prefixMatcher()
{
    return {};
}

IHttpResponseWare* IHttpResponseWare::prefixCreate(const std::string &)
{
    IGlobalAbort::abortUnVisibleMethod();
    return nullptr;
}

$PackageWebCoreEnd
