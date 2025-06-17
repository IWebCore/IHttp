#include "IRequest.h"

#include "core/abort/IGlobalAbort.h"
#include "core/unit/IObjectMemoryPoolUnit.h"
#include "http/IHttpConstant.h"
#include "http/detail/IHttpRequestImpl.h"
#include "http/IHttpCookieJar.h"
#include "http/IHttpSession.h"
#include "http/IHttpHeaderJar.h"
#include "tcp/ITcpConnection.h"

$PackageWebCoreBegin

namespace detail
{
    static IObjectMemoryPoolUnit<IHttpRequestImpl> s_pool;
}

IRequest::IRequest(ITcpConnection& connection, int resolverFactoryId)
    : ITcpResolver(connection, resolverFactoryId)
{
    m_impl = detail::s_pool.allocate(*this);
}

IRequest::~IRequest()
{
    IHttpManage::instance().invokeFilters<IHttpFilterWare::Type::completion>(*this);
    detail::s_pool.deallocate(m_impl);
}

IStringView IRequest::operator[](const char *header) const
{
    return m_impl->m_headerJar.getRequestHeaderValue(header);
}

IStringView IRequest::operator[](const std::string &header) const
{
    return m_impl->m_headerJar.getRequestHeaderValue(header);
}

IStringView IRequest::operator[](const IString &header) const
{
    return m_impl->m_headerJar.getRequestHeaderValue(header);
}

IStringView IRequest::operator[](const QString &header) const
{
    return m_impl->m_headerJar.getRequestHeaderValue(header.toUtf8());
}

bool IRequest::isSessionExist() const
{
    return m_impl->isSessionExist();
}

const IHttpCookieJar& IRequest::cookieJar() const
{
    return m_impl->m_cookieJar;
}

IHttpSession& IRequest::session()
{
    return m_impl->session();
}

const IHttpHeaderJar& IRequest::headerJar() const
{
    return m_impl->m_headerJar;
}

const IHttpMultiPartJar& IRequest::multiPartJar() const
{
    return m_impl->m_multiPartJar;
}

IHttpRequestImpl& IRequest::impl() const
{
    return *m_impl;
}

IStringView IRequest::url() const
{
    return m_impl->m_reqRaw.m_url;
}

IHttpVersion IRequest::version() const
{
    return m_impl->m_reqRaw.m_httpVersion;
}

IHttpMime IRequest::mime() const
{
    return m_impl->m_reqRaw.m_mime;
}

IHttpMethod IRequest::method() const
{
    return m_impl->m_reqRaw.m_method;
}

std::size_t IRequest::contentLength() const
{
    return m_impl->m_reqRaw.m_contentLength;
}

IStringView IRequest::contentType() const
{
    return m_impl->m_headerJar.getRequestHeaderValue(IHttpHeader::ContentType);
}

IStringView IRequest::content() const
{
    return m_impl->m_reqRaw.m_body;
}

const QMap<IStringView, IStringView> &IRequest::pathParameters() const
{
    return m_impl->m_reqRaw.m_paths;
}

const QMap<IStringView, IStringView> &IRequest::queryParameters() const
{
    return m_impl->m_reqRaw.m_queries;
}

const QMap<IStringView, IStringView> &IRequest::bodyFormParameters() const
{
    return m_impl->m_reqRaw.m_forms;
}

const std::vector<IHttpMultiPart> &IRequest::bodyMultiParts() const
{
    return m_impl->m_reqRaw.m_multiParts;
}

const IJson& IRequest::bodyJson() const
{
    return m_impl->m_reqRaw.m_json;
}

bool IRequest::isValid() const
{
    return m_impl->m_isValid;
}

void IRequest::setInvalid(const IHttpInvalidWare& ware) const
{
    m_impl->setInvalid(ware);
}

void IRequest::startRead()
{
    m_readState = ReadState::Reading;
    m_connection.doReadDataUtil(IHttp::NEW_PART);
}

void IRequest::resolve()
{
    if(m_writeCount == 0){
        m_connection.doWriteFinished();
        return;
    }
    m_impl->parseData();
}

void IRequest::startWrite()
{
    IHttpManage::instance().invokeFilters<IHttpFilterWare::Type::PostHandle>(*this);

    m_impl->m_respRaw.prepareResult(*m_impl);
    m_writeState = WriteState::Writing;
    m_connection.doWrite(this);
}

std::vector<asio::const_buffer> IRequest::getOutput()
{
    return m_impl->m_respRaw.m_result;
}

IStringView IRequest::stash(const char *data) const
{
    return m_impl->stash(data);
}

IStringView IRequest::stash(const QString &data) const
{
    return m_impl->stash(data);
}

IStringView IRequest::stash(std::string &&data) const
{
    return m_impl->stash(std::move(data));
}

IStringView IRequest::stash(const std::string &data) const
{
    return m_impl->stash(data);
}

IStringView IRequest::stash(QByteArray &&data) const
{
    return m_impl->stash(std::move(data));
}

IStringView IRequest::stash(const QByteArray &data) const
{
    return m_impl->stash(data);
}

IStringView IRequest::stash(IString &&data) const
{
    return m_impl->stash(std::move(data));
}

IStringView IRequest::stash(const IString &data) const
{
    return m_impl->stash(data);
}

IStringView IRequest::stash(IStringView data) const
{
    return m_impl->stash(data);
}

$PackageWebCoreEnd
