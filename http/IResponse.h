#pragma once

#include "core/util/IHeaderUtil.h"
#include "http/biscuits/IHttpStatus.h"
#include "http/biscuits/IHttpVersion.h"
#include "http/biscuits/IHttpMime.h"
#include "http/biscuits/IHttpHeader.h"
#include "http/invalid/IHttpInvalidWare.h"

$PackageWebCoreBegin

class IRequest;
class IHttpResponseWare;
class IHttpRequestImpl;
class IHttpCookiePart;
class IHttpCookieJar;
class IHttpSession;
class IHttpHeaderJar;
class IResponse
{
public:
    IResponse() = delete;
    IResponse(const IResponse&) = delete;
    IResponse(IResponse &&) = delete;
    IResponse& operator=(const IResponse&) = delete;
    IResponse &operator=(IResponse &&) = delete;
    explicit IResponse(IRequest& request);
    ~IResponse() = default;

public:
    IHttpCookieJar& cookieJar();
    IHttpSession& session();
    IHttpHeaderJar& headerJar();

public:
//    IResponseHeader operator[](const IString& header) const;
    IResponse& setHeader(IStringView key, IStringView value);

    IResponse& setStatus(IHttpStatus statusCode);
    IResponse& setStatus(int statusCode);

    IResponse& setMime(IHttpMime mime);
    IResponse& setMime(IString mime);

    IResponse& addCookie(IHttpCookiePart cookiePart);

    template<typename T>
    IResponse& setContent(T strData);
//    IResponse& setContent(const IHttpResponseWare& ware);    // 对于这个，可以多思考一下，使用引用怎么样
//    IResponse& setContent(const IHttpInvalidWare& ware);
    IResponse& setResponseWare(const IHttpResponseWare&);
    IResponse& setInvalidWare(const IHttpInvalidWare& ware);

    void setInvalid(const IHttpInvalidWare& ware);

    IHttpVersion version() const;
    IStringView mime() const;
    IHttpStatus status() const;
    const IHttpHeader& headers() const;

private:
    IHttpRequestImpl& m_impl;
};

// not recommend!!! this will produce copied data
template<typename T>
IResponse& IResponse::setContent(T data)
{
    using Type = std::remove_cv_t<std::remove_reference_t<T>>;

    if constexpr (std::is_base_of_v<IHttpResponseWare, Type>){
        return setResponseWare(data);
    }
    if constexpr (std::is_base_of_v<IHttpInvalidWare, Type>){
        return setInvalidWare(data);
    }
    qFatal("your content do not meet the need");
    return *this;
}

$PackageWebCoreEnd

