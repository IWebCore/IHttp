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
#include <cstddef> // for offsetof

$PackageWebCoreBegin

static const IString ServerHeader = "Server: IWebCore\r\n";

namespace detail
{

IStringView generateStatusCode(int status)
{
    static std::once_flag flag;
    static std::vector<IString> statusString;
    std::call_once(flag, [](){
        statusString.reserve(1025);
        for(int i=0; i<1025; i++){
            statusString.emplace_back(std::to_string(i));
        }
    });
    return statusString[status].m_view;
}

void generateFirstLine(IHttpResponseRaw::RetType& ret, IHttpRequestImpl& impl)
{
    ret.emplace_back(IHttpVersionUtil::toString(impl.m_reqRaw.m_httpVersion));
    ret.emplace_back(IHttp::SPACE);
    ret.emplace_back(generateStatusCode(static_cast<int>(impl.m_respRaw.m_status)));
    ret.emplace_back(IHttp::SPACE);
    ret.emplace_back(IHttpStatusUtil::toStringDescription(impl.m_respRaw.m_status));
    ret.emplace_back(IHttp::NEW_LINE);
}

void generateCookieHeaders(IHttpResponseRaw::RetType& ret, IHttpRequestImpl& impl)
{
    const auto& cookies = impl.m_respRaw.m_cookies;
    int size = 1 + 18* cookies.size() + ret.size();
    if(ret.capacity() < size){
        ret.reserve(size * 2);
    }

    for(const auto& cookie : impl.m_respRaw.m_cookies){
        auto vals = cookie.toHeaderString();
        for(auto val : vals){
            ret.push_back(val);
        }
    }
}

void generateHeadersContent(IHttpResponseRaw::RetType& ret, IHttpRequestImpl& m_raw)
{
    auto& headers = m_raw.m_respRaw.m_headers;
    std::size_t size = (1 + 4* headers.m_header.size() + ret.size());
    if(ret.capacity() < size){
        ret.reserve(size * 2);
    }

    ret.push_back(ServerHeader);
    for(const auto& pair : headers.m_header){
        ret.push_back(pair.first);
        ret.push_back(IHttp::COMMA_SPACE);
        ret.push_back(pair.second);
        ret.push_back(IHttp::NEW_LINE);
    }
}

}

IHttpResponseRaw::IHttpResponseRaw(const IHttpResponseRaw &rhs)
    : IStringViewStash(rhs)
{
    IStringViewStash::operator=(rhs);

    m_isValid = rhs.m_isValid;
    m_mime = rhs.m_mime;
    m_status = rhs.m_status;
    m_cookies = rhs.m_cookies;
    for(auto content : m_contents){
        m_contents.push_back(new IHttpResponseContent(*content));
    }
}

IHttpResponseRaw &IHttpResponseRaw::operator=(const IHttpResponseRaw &rhs)
{
    IStringViewStash::operator=(rhs);

    m_isValid = rhs.m_isValid;
    m_mime = rhs.m_mime;
    m_status = rhs.m_status;
    m_cookies = rhs.m_cookies;
    for(auto content : m_contents){
        m_contents.push_back(new IHttpResponseContent(*content));
    }

    return *this;
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

void IHttpResponseRaw::setContent(IHttpInvalidWare &&ware)
{
    setContent(ware);
}

void IHttpResponseRaw::setContent(const IHttpResponseWare &response)
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

    while(!response.m_raw->m_contents.empty()){
        setContent(response.m_raw->m_contents.front());
        response.m_raw->m_contents.pop_front();
    }
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

void IHttpResponseRaw::setCookie(const IString &key, const IString &value, int maxAge)
{
    IHttpCookiePart cookie(key, value, maxAge);
    m_cookies.emplace_back(std::move(cookie));
}

void IHttpResponseRaw::prepareResult(IHttpRequestImpl& impl)
{
    while(!m_contents.empty() && m_contents.back()->m_processor){
        m_contents.back()->m_processor(*(m_contents.back()), *this);
    }

    if(!m_contents.empty()){
        m_target = m_contents.back()->m_content;
    }

    prepareHeaders(impl);

    m_result.reserve(1024);
    detail::generateFirstLine(m_result, impl);          // first line
    detail::generateHeadersContent(m_result, impl);     // headers
    detail::generateCookieHeaders(m_result, impl);      // cookies
    m_result.push_back(IHttp::NEW_LINE);

    if(!m_target.empty()  && impl.m_reqRaw.m_method != IHttpMethod::HEAD){   // body
        m_result.push_back(m_target);
    }
}

std::vector<asio::const_buffer> IHttpResponseRaw::getResult()
{
    std::vector<asio::const_buffer> buffers;
    buffers.reserve(m_result.size());

    if (false) {
        std::memcpy(
            buffers.data(),
            m_result.data(),
            m_result.size() * sizeof(asio::const_buffer)
        );
    } else {
        for (const auto& sv : m_result) {
            buffers.emplace_back(sv.data(), sv.size());
        }
    }

    return buffers;
}

void IHttpResponseRaw::prepareHeaders(IHttpRequestImpl& impl)
{
    // content-length and mime
    setHeader(&IHttpHeader::ContentLength, std::to_string(m_target.length()));
    if(!m_target.empty()){
        if(!m_headers.contain(IHttpHeader::ContentType) && !m_mime.isEmpty()){
            m_headers.insert(&IHttpHeader::ContentType, &m_mime);
        }
    }

    // keep alive
    if(impl.m_connection.m_keepAlive){
        IString hello(&"hello");
        IString world("world");
        setHeader(&IHttpHeader::Connection, &"keep-alive");
        setHeader(&IHttpHeader::KeepAlive, &"timeout=10, max=50");
    }
}

$PackageWebCoreEnd
