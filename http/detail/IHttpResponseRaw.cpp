#include "IHttpResponseRaw.h"
#include "http/IHttpConstant.h"
#include "http/biscuits/IHttpHeader.h"
#include "http/biscuits/IHttpVersion.h"
#include "http/detail/IHttpRequestImpl.h"
#include "http/response/IHttpResponseWare.h"
#include "http/invalid/IHttpInvalidWare.h"
#include "http/response/content/IHttpInvalidReponseContent.h"
#include "http/response/content/IHttpFileResponseContent.h"
#include "tcp/ITcpConnection.h"

$PackageWebCoreBegin

static const IString ServerHeader = "Server: IWebCore\r\n";

namespace detail
{
std::vector<IStringView> generateFirstLine(IHttpRequestImpl& impl)
{
    std::vector<IStringView> ret;
    ret.push_back(IHttpVersionUtil::toString(impl.m_reqRaw.m_httpVersion).m_view);
    ret.push_back(IHttp::SPACE);
    ret.push_back(IHttpStatusUtil::toStringNumber(impl.m_respRaw.m_status).m_view);
    ret.push_back(IHttp::SPACE);
    ret.push_back(IHttpStatusUtil::toStringDescription(impl.m_respRaw.m_status).m_view);
    ret.push_back(IHttp::NEW_LINE);

    return ret;
}

std::vector<IStringView> generateCookieHeaders(IHttpRequestImpl& impl)
{
    std::vector<IStringView> ret;
    for(const auto& cookie : impl.m_respRaw.m_cookies){
        auto vals = cookie.toHeaderString();
        for(auto val : vals){
            ret.push_back(val);
        }
    }

    return ret;
}

std::vector<IStringView> generateHeadersContent(IHttpRequestImpl& m_raw, std::size_t contentSize)
{
    auto& headers = m_raw.m_respRaw.m_headers;

    headers.insert(IHttpHeader::ContentLength, IString(std::to_string(contentSize)));
    if(contentSize != 0){
        if(!headers.contain(IHttpHeader::ContentType) && !m_raw.m_respRaw.m_mime.isEmpty()){
            headers.insert(IHttpHeader::ContentType, m_raw.m_respRaw.m_mime);
        }
    }

    if(m_raw.m_connection.m_keepAlive){
        headers.insert(IHttpHeader::Connection, "keep-alive");
        headers.insert(IHttpHeader::KeepAlive, "timeout=10, max=50");
    }

    std::vector<IStringView> ret;
    ret.push_back(ServerHeader.m_view);

    for(const auto& pair : headers.m_header){
        ret.push_back(pair.first);
        ret.push_back(IHttp::COMMA_SPACE);
        ret.push_back(pair.second);
        ret.push_back(IHttp::NEW_LINE);
    }

    return ret;
}

}

IHttpResponseRaw::~IHttpResponseRaw()
{
    while(!m_contents.empty()){
        delete m_contents.front();
        m_contents.pop_front();
    }
}

void IHttpResponseRaw::setHeader(IString key, IString value)
{
    m_headers.insert(std::move(key), std::move(value));
}

void IHttpResponseRaw::setMime(IHttpMime mime)
{
    this->m_mime = IHttpMimeUtil::toString(mime);
}

void IHttpResponseRaw::setContent(const IHttpInvalidWare& ware)
{
    m_status = ware.status;
    m_mime = IHttpMimeUtil::toString(ware.mime);
    setContent(new IHttpInvalidReponseContent(ware));
}

void IHttpResponseRaw::setContent(IHttpResponseContent *ware)
{
    m_contents.push_back(ware);

    auto invalidWare = dynamic_cast<IHttpInvalidReponseContent*>(ware);
    if(invalidWare){
        this->m_isValid = false;
    }
}

void IHttpResponseRaw::setCookie(IHttpCookiePart &&cookie)
{
    m_cookies.emplace_back(std::move(cookie));
}

void IHttpResponseRaw::setCookie(const IHttpCookiePart &cookie)
{
    m_cookies.push_back(cookie);
}

void IHttpResponseRaw::setCookie(const IString &key, const IString &value)
{
    m_cookies.emplace_back(key, value);
}

void IHttpResponseRaw::prepareResult(IHttpRequestImpl& impl)
{
    // 这一步是做特殊的处理，将数据封装起来。
    while(!m_contents.empty() && m_contents.back()->m_processor){
        m_contents.back()->m_processor(*(m_contents.back()), *this);
    }

    // first line
    auto firstLine = detail::generateFirstLine(impl);
    for(auto view : firstLine){
        m_result.push_back(view.toAsioBuffer());
    }

    IStringView content{};
    if(!m_contents.empty()){
        content = m_contents.back()->m_content.m_view;
    }

    // headers
    auto headers = detail::generateHeadersContent(impl, content.size());
    for(auto view : headers){
        m_result.push_back(view.toAsioBuffer());
    }

    auto cookies = detail::generateCookieHeaders(impl);
    for(auto view : cookies){
        m_result.push_back(view.toAsioBuffer());
    }

    m_result.push_back(IHttp::NEW_LINE.toAsioBuffer());
    if(!content.empty()  && impl.m_reqRaw.m_method != IHttpMethod::HEAD){
        m_result.push_back(content.toAsioBuffer());
    }
}

void IHttpResponseRaw::setResponseWare(const IHttpResponseWare &response)
{
    if(!response.mime().isEmpty()){
        m_mime = response.mime();
    }
    if(response.status() != m_status){
        m_status = response.status();
    }

    if(!response.headers().isEmpty()){
        for(auto& [key, val] : response.headers().m_header){
            setHeader(key, val);
        }
    }

    if(!response.m_raw->m_cookies.empty()){
        for(auto cookie : response.m_raw->m_cookies){
            this->m_cookies.push_back(cookie);
        }
        response.m_raw->m_cookies.clear();
    }

    // NOTE: this break the const constrait, but its safe
    while(!response.m_raw->m_contents.empty()){
        setContent(response.m_raw->m_contents.front());
        response.m_raw->m_contents.pop_front();
    }
}

$PackageWebCoreEnd
