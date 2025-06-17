#include "IHttpHeaderJar.h"
#include "core/abort/IGlobalAbort.h"
#include "http/IHttpConstant.h"
#include "http/detail/IHttpRequestRaw.h"
#include "http/detail/IHttpResponseRaw.h"
#include "http/detail/IHttpRequestImpl.h"

$PackageWebCoreBegin

IStringViewList IHttpHeaderJar::requestHeaderKeys() const
{
    IStringViewList ret;
    const auto& headers = m_impl.m_reqRaw.m_headers;
    for(auto it=headers.begin(); it!= headers.end(); it++){
        ret.push_back(it.key());
    }
    return ret;
}

bool IHttpHeaderJar::containRequestHeaderKey(const IString& key) const
{
    const auto& keys = requestHeaderKeys();
    return std::find_if(keys.begin(), keys.end(), [&](IStringView data){
        return data.equalIgnoreCase(key.m_view);
    }) != keys.end();
}

IStringView IHttpHeaderJar::getRequestHeaderValue(const IString& view) const
{
    const auto& headers = m_impl.m_reqRaw.m_headers;
    for(auto it=headers.begin(); it!= headers.end(); it++){
        if(it.key().equalIgnoreCase(view)){
            return it.value();
        }
    }

    return IHttp::EMPTY;
}

IHttpHeader &IHttpHeaderJar::responseHeaders()
{
    return m_impl.m_respRaw.m_headers;
}

IStringViewList IHttpHeaderJar::responseHeaderKeys() const
{
    return m_impl.m_respRaw.m_headers.keys();
}

bool IHttpHeaderJar::containResponseHeaderKey(IStringView key) const
{
    return m_impl.m_respRaw.m_headers.contain(key);
}

void IHttpHeaderJar::addResponseHeader(IString key, IString value)
{
    m_impl.m_respRaw.m_headers.insert(std::move(key), std::move(value));
}

void IHttpHeaderJar::deleteReponseHeader(IStringView key)
{
    m_impl.m_respRaw.m_headers.remove(key);
}

$PackageWebCoreEnd
