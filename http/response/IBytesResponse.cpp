#include "IBytesResponse.h"
#include "http/response/content/IHttpResponseContent.h"

$PackageWebCoreBegin

template class IHttpResponseInterface<IBytesResponse>;

IBytesResponse::IBytesResponse()
{
    m_raw->setMime(IHttpMime::APPLICATION_OCTET_STREAM);
}

IBytesResponse::IBytesResponse(const char *data)
    : IBytesResponse(QByteArray(data))
{
}

IBytesResponse::IBytesResponse(const QString &data)
{
    m_raw->setMime(IHttpMime::APPLICATION_OCTET_STREAM);
    m_raw->setContent(new IHttpResponseContent(data.toUtf8()));
}

IBytesResponse::IBytesResponse(QByteArray &&array)
{
    m_raw->setMime(IHttpMime::APPLICATION_OCTET_STREAM);
    m_raw->setContent(new IHttpResponseContent(std::move(array)));
}

IBytesResponse::IBytesResponse(const QByteArray &array)
{
    m_raw->setMime(IHttpMime::APPLICATION_OCTET_STREAM);
    m_raw->setContent(new IHttpResponseContent(array));
}

IBytesResponse::IBytesResponse(std::string &&data)
{
    m_raw->setMime(IHttpMime::APPLICATION_OCTET_STREAM);
    m_raw->setContent(new IHttpResponseContent(std::move(data)));
}

IBytesResponse::IBytesResponse(const std::string & data)
{
    m_raw->setMime(IHttpMime::APPLICATION_OCTET_STREAM);
    m_raw->setContent(new IHttpResponseContent(data));
}

IBytesResponse::IBytesResponse(IString && data)
{
    m_raw->setMime(IHttpMime::APPLICATION_OCTET_STREAM);
    m_raw->setContent(new IHttpResponseContent(std::move(data)));
}

IBytesResponse::IBytesResponse(const IString &data)
{
    m_raw->setMime(IHttpMime::APPLICATION_OCTET_STREAM);
    m_raw->setContent(new IHttpResponseContent(data));
}

std::string IBytesResponse::prefixMatcher()
{
    return "$bytes:";
}

IBytesResponse operator"" _bytes(const char* str, size_t size){
    return QByteArray::fromRawData(str, static_cast<int>(size));
}

$PackageWebCoreEnd
