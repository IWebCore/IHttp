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
class IHttpResponseRaw : public IStringViewStash
{
    friend class IHttpRequestImpl;
public:
    IHttpResponseRaw() = default;
    ~IHttpResponseRaw();

public:
    void setHeader(IString key, IString value);
    void setMime(IHttpMime m_mime);

    void setContent(const IHttpInvalidWare& ware);
    void setContent(IHttpResponseContent*);

    void setCookie(IHttpCookiePart&&);
    void setCookie(const IHttpCookiePart&);
    void setCookie(const IString& key, const IString& value);

private:
    void setResponseWare(const IHttpResponseWare&);

public:
    void prepareResult(IHttpRequestImpl&);

public:
    bool m_isValid{true};
    IString m_mime;
    IHttpStatus m_status {IHttpStatus::OK_200};
    IHttpHeader m_headers;
    std::list<IHttpCookiePart> m_cookies;
    std::list<IHttpResponseContent*> m_contents;
    std::vector<asio::const_buffer> m_result;
};

$PackageWebCoreEnd
