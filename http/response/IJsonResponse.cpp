#include "IJsonResponse.h"
#include "IHttpResponseManage.h"
#include "http/detail/IHttpResponseRaw.h"

$PackageWebCoreBegin

IJsonResponse::IJsonResponse()
{
    m_raw->setMime(IHttpMime::APPLICATION_JSON_UTF8); // this must be initialized, the only return type;
}

IJsonResponse::IJsonResponse(const char * value)
{
    m_raw->setMime(IHttpMime::APPLICATION_JSON_UTF8);
    m_raw->setContent(new IHttpResponseContent(std::string(value)));
}

IJsonResponse::IJsonResponse(const QString& value) : IHttpResponseInterface()
{
    m_raw->setMime(IHttpMime::APPLICATION_JSON_UTF8);
    m_raw->setContent(new IHttpResponseContent(value.toStdString()));
}

IJsonResponse::IJsonResponse(IJson && json)
{
    m_raw->setMime(IHttpMime::APPLICATION_JSON_UTF8);
    m_raw->setContent(new IHttpResponseContent(json.dump()));
}

IJsonResponse::IJsonResponse(const IJson &json)
{
    m_raw->setMime(IHttpMime::APPLICATION_JSON_UTF8);
    m_raw->setContent(new IHttpResponseContent(json.dump()));
}

IJsonResponse::IJsonResponse(std::string&& value)
{
    m_raw->setMime(IHttpMime::APPLICATION_JSON_UTF8);
    m_raw->setContent(new IHttpResponseContent(std::move(value)));
}

IJsonResponse::IJsonResponse(IString &&value)
{
    m_raw->setMime(IHttpMime::APPLICATION_JSON_UTF8);
    m_raw->setContent(new IHttpResponseContent(std::move(value)));
}

IJsonResponse::IJsonResponse(const IString &value)
{
    m_raw->setMime(IHttpMime::APPLICATION_JSON_UTF8);
    m_raw->setContent(new IHttpResponseContent(value));
}

IJsonResponse::IJsonResponse(QByteArray && value)
{
    m_raw->setMime(IHttpMime::APPLICATION_JSON_UTF8);
    m_raw->setContent(new IHttpResponseContent(std::move(value)));
}

IJsonResponse::IJsonResponse(const QByteArray & value)
{
    m_raw->setMime(IHttpMime::APPLICATION_JSON_UTF8);
    m_raw->setContent(new IHttpResponseContent(value));
}

std::string IJsonResponse::prefixMatcher()
{
    return "$json:";
}

IJsonResponse operator"" _json(const char *str, size_t size)
{
    auto data = std::string(str, static_cast<int>(size));
    return IJsonResponse(std::move(data));
}

$PackageWebCoreEnd
