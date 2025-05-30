#include "IPlainTextResponse.h"
#include "http/detail/IHttpResponseRaw.h"
#include "http/response/content/IHttpResponseContent.h"

$PackageWebCoreBegin

IPlainTextResponse::IPlainTextResponse()
{
    m_raw->setMime(IHttpMime::TEXT_PLAIN_UTF8);
}

IPlainTextResponse::IPlainTextResponse(const char *value)
{
    m_raw->setContent(new IHttpResponseContent(value));
    m_raw->setMime(IHttpMime::TEXT_PLAIN_UTF8);
}

IPlainTextResponse::IPlainTextResponse(const QString &value)
{
    m_raw->setContent(new IHttpResponseContent(value.toUtf8()));
    m_raw->setMime(IHttpMime::TEXT_PLAIN_UTF8);
}

IPlainTextResponse::IPlainTextResponse(std::string &&value)
{
    m_raw->setContent(new IHttpResponseContent(std::move(value)));
    m_raw->setMime(IHttpMime::TEXT_PLAIN_UTF8);
}

IPlainTextResponse::IPlainTextResponse(const std::string &value)
{
    m_raw->setContent(new IHttpResponseContent(value));
    m_raw->setMime(IHttpMime::TEXT_PLAIN_UTF8);
}

IPlainTextResponse::IPlainTextResponse(QByteArray &&value)
{
    m_raw->setContent(new IHttpResponseContent(std::move(value)));
    m_raw->setMime(IHttpMime::TEXT_PLAIN_UTF8);
}

IPlainTextResponse::IPlainTextResponse(const QByteArray &value)
{
    m_raw->setContent(new IHttpResponseContent(value));
    m_raw->setMime(IHttpMime::TEXT_PLAIN_UTF8);
}

IPlainTextResponse::IPlainTextResponse(IString &&value)
{
    m_raw->setContent(new IHttpResponseContent(std::move(value)));
    m_raw->setMime(IHttpMime::TEXT_PLAIN_UTF8);
}

IPlainTextResponse::IPlainTextResponse(const IString &value)
{
    m_raw->setContent(new IHttpResponseContent(value));
    m_raw->setMime(IHttpMime::TEXT_PLAIN_UTF8);
}

std::string IPlainTextResponse::prefixMatcher()
{
    return "$text:";
}

IPlainTextResponse operator"" _text(const char* str, size_t size)
{
    return QString::fromLocal8Bit(str, static_cast<int>(size));
}


$PackageWebCoreEnd
