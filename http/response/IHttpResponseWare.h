#pragma once

#include "core/util/IHeaderUtil.h"
#include "http/biscuits/IHttpMime.h"
#include "http/biscuits/IHttpStatus.h"
#include "http/detail/IHttpResponseRaw.h"
#include "http/detail/IHttpResponseHeader.h"

$PackageWebCoreBegin

class IHttpResponseWare
{
public:
    IHttpResponseWare();
    virtual ~IHttpResponseWare();

    IHttpResponseWare(IHttpResponseWare&&);
    IHttpResponseWare(const IHttpResponseWare &);
    IHttpResponseWare& operator =(const IHttpResponseWare&);
    IHttpResponseWare& operator =(IHttpResponseWare&&);
    IHttpResponseHeader operator[](const IString& header);

public:
    const IString& mime() const;
    void setMime(IHttpMime);
    
    IHttpStatus status() const;
    void setStatus(IHttpStatus statusCode);

    const IHttpHeader& headers() const;
    IHttpHeader& headers();

    void setHeader(IStringView key, IStringView value);

    void setCookie(IHttpCookiePart&&);
    void setCookie(const IHttpCookiePart&);
    void setCookie(const IString& key, const IString& value);
    void setCookie(const IString& key, const IString& value, int maxAge);

    void setContent(IString&& value);
    void setContent(const IString& value);
    void setContent(const IHttpInvalidWare&);

    virtual std::string prefixMatcher();
    virtual IHttpResponseWare* prefixCreate(const std::string&);

public:
    IHttpResponseRaw* m_raw{nullptr};
};

$PackageWebCoreEnd
