#include "IHttpFileResponseContent.h"
#include "core/util/IFileUtil.h"
#include "core/util/ICodecUtil.h"
#include "http/IHttpConstant.h"
#include "http/biscuits/IHttpMime.h"
#include "http/detail/IHttpResponseRaw.h"
#include "http/invalid/IHttpNotFoundInvalid.h"
#include "http/invalid/IHttpInternalErrorInvalid.h"
#include "http/response/content/IHttpInvalidReponseContent.h"

$PackageWebCoreBegin

namespace detail
{
    static void fileResponseProcessor(const IHttpResponseContent& content, IHttpResponseRaw& raw);
    static IString createDispoisition(const IString&);
    IStringView findMime(const IString&);
}

IHttpFileResponseContent::IHttpFileResponseContent(IString && value)
    : IHttpResponseContent(std::move(value))
{
    m_processor = detail::fileResponseProcessor;
}

IHttpFileResponseContent::IHttpFileResponseContent(const IString & value)
    : IHttpResponseContent(value)
{
    m_processor = detail::fileResponseProcessor;
}

void detail::fileResponseProcessor(const IHttpResponseContent &content, IHttpResponseRaw &raw)
{
    QFileInfo info(content.m_content.toQString());
    if(!info.exists()){
        raw.setMime(IHttpMime::TEXT_PLAIN_UTF8);
        raw.setContent(IHttpInternalErrorInvalid());
        return;
    }

    auto data = IFileUtil::readFileAsByteArray(info.absoluteFilePath());
    raw.setContent(new IHttpResponseContent(std::move(data)));
    if(true && content.m_attribute
            && content.m_attribute->contains(IHttpFileResponseContent::ContentDispoistion)
            && (content.m_attribute->operator [](IHttpFileResponseContent::ContentDispoistion)) == IHttp::TRUE_STR)
    {
        raw.m_headers.insert(&IHttpHeader::ContentDisposition, detail::createDispoisition(content.m_content));
    }

    if(false || !content.m_attribute
             || !content.m_attribute->contains(IHttpFileResponseContent::ContentTypeEnabled)
             || (content.m_attribute->operator [](IHttpFileResponseContent::ContentTypeEnabled)) != IHttp::FALSE_STR)
    {
        auto mime = detail::findMime(content.m_content);
        if(mime != IHttpMimeUtil::MIME_UNKNOWN_STRING){
            raw.m_headers.insert(&IHttpHeader::ContentType, mime);
        }
    }
}

IString detail::createDispoisition(const IString & data)
{
    auto fileName  = QFileInfo(data.toQString()).fileName();
    return (QString("attachment;filename=").append(ICodecUtil::urlEncode(fileName))).toUtf8();
}

IStringView detail::findMime(const IString& data)
{
    auto suffix = QFileInfo(data.toQString()).suffix();
    return IHttpMimeUtil::getSuffixMime(IString(suffix.toUtf8()));
}

$PackageWebCoreEnd
