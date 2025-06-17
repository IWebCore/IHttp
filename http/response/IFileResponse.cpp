#include "IFileResponse.h"
#include "core/util/ICodecUtil.h"
#include "core/util/IFileUtil.h"
#include "core/config/IProfileImport.h"
#include "http/IHttpConstant.h"
#include "http/biscuits/IHttpMime.h"
#include "http/response/content/IHttpFileResponseContent.h"

$PackageWebCoreBegin

IFileResponse::IFileResponse(const char *data)
    : IFileResponse(IString(data))
{
}

IFileResponse::IFileResponse(const QString &data)
    : IFileResponse(IString(data.toUtf8()))
{
}

IFileResponse::IFileResponse(IString &&path)
{
    m_raw->setContent(new IHttpFileResponseContent(std::move(path)));
    m_raw->setMime(IHttpMime::APPLICATION_OCTET_STREAM);
}

IFileResponse::IFileResponse(const IString &path)
{
    m_raw->setContent(new IHttpFileResponseContent(path));
    m_raw->setMime(IHttpMime::APPLICATION_OCTET_STREAM);
}

IFileResponse::IFileResponse(const std::string &path)
    : IFileResponse(IString(path))
{
}

IFileResponse::IFileResponse(std::string &&path)
    : IFileResponse(IString(std::move(path)))
{
}

void IFileResponse::setContent(const IHttpInvalidWare &ware)
{
    IHttpResponseInterface::setContent(ware);
}

void IFileResponse::enableContentDisposition()
{
    if(m_raw->m_contents.size() != 0){
        auto ptr = dynamic_cast<IHttpFileResponseContent*>(m_raw->m_contents.back());
        if(ptr){
            ptr->setAttribute(&IHttpFileResponseContent::ContentDispoistion, IHttp::TRUE_STR);
        }
    }
}

std::string IFileResponse::prefixMatcher()
{
    return "$file:";
}

IFileResponse operator"" _file(const char* str, size_t size)
{
    QByteArray array(str, static_cast<int>(size));
    QString name(array);

    IFileResponse response(name);
    return response;
}

$PackageWebCoreEnd
