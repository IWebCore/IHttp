#pragma once

#include "core/base/IStringViewStash.h"
#include "core/util/IHeaderUtil.h"
#include "http/biscuits/IHttpMime.h"
#include "http/biscuits/IHttpStatus.h"
#include "http/biscuits/IHttpHeader.h"
#include "http/IHttpCookiePart.h"
#include "http/response/content/IHttpResponseContent.h"

$PackageWebCoreBegin

class IHttpRequestImpl;
class IHttpResponseWare;
class IHttpInvalidWare;
class IHttpResponseRaw : public IStringViewStash        // TODO: check stash whether worked when copy
{
    friend class IHttpRequestImpl;
public:
    using RetType = std::vector<IStringView>;
public:
    IHttpResponseRaw() = default;
    IHttpResponseRaw(IHttpResponseRaw&& rhs) = default;
    IHttpResponseRaw(const IHttpResponseRaw& rhs);
    IHttpResponseRaw& operator=(IHttpResponseRaw&&rhs) = default;
    IHttpResponseRaw& operator=(const IHttpResponseRaw&rhs);
    ~IHttpResponseRaw();

public:
    void setHeader(IString key, IString value);
    void setMime(IHttpMime m_mime);

    void setContent(const IHttpInvalidWare& ware);
    void setContent(IHttpInvalidWare&& ware);
    void setContent(IHttpResponseContent*);

    void setCookie(IHttpCookiePart&&);
    void setCookie(const IHttpCookiePart&);
    void setCookie(const IString& key, const IString& value);
    void setResponseWare(const IHttpResponseWare&);

public:
    void prepareResult(IHttpRequestImpl&);

public:
    std::vector<asio::const_buffer> getResult();

private:
    void prepareHeaders(IHttpRequestImpl& impl);

public:
    bool m_isValid{true};
    IString m_mime;
    IHttpStatus m_status {IHttpStatus::OK_200};
    IHttpHeader m_headers;
    IStringView m_target;   // 将要写入的内容
    std::list<IHttpCookiePart> m_cookies;
    std::list<IHttpResponseContent*> m_contents;
    RetType m_result;
};

$PackageWebCoreEnd
