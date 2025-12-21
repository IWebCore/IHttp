#include "IHttpResponseInterface.h"
#include "core/abort/IAbortInterface.h"
#include "http/response/content/IHttpResponseContent.h"

$PackageWebCoreBegin

template class IHttpResponseInterface<IStatusResponse>;

IStatusResponse::IStatusResponse(const QString& num)
{
    m_raw->m_status = IHttpStatus(num.toInt());
}

IStatusResponse::IStatusResponse(const std::string & data)
    : IStatusResponse(QString::fromStdString(data))
{
}

IStatusResponse::IStatusResponse(int code, const QString& errorMsg)
{
    m_raw->m_status = IHttpStatus(code);
    if(!errorMsg.isEmpty()){
        m_raw->setMime(IHttpMime::TEXT_PLAIN_UTF8);
        m_raw->setContent(new IHttpResponseContent(errorMsg.toUtf8()));
    }
}

IStatusResponse::IStatusResponse(IHttpStatus status, const QString &errorMsg)
{
    m_raw->m_status = status;
    if(!errorMsg.isEmpty()){
        m_raw->setMime(IHttpMime::TEXT_PLAIN_UTF8);
        m_raw->setContent(new IHttpResponseContent(errorMsg.toUtf8()));
    }
}

std::string IStatusResponse::prefixMatcher()
{
    return "$status:";
}

IStatusResponse operator"" _status(unsigned long long int value)
{
    return IStatusResponse(value);
}

$PackageWebCoreEnd
