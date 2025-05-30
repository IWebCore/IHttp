#pragma once

#include <type_traits>
#include "IHttpResponseInterface.h"
#include "IHttpResponsePreProcessor.h"
#include "core/util/IJsonUtil.h"
#include "http/response/content/IHttpResponseContent.h"

$PackageWebCoreBegin

class IJsonResponse : public IHttpResponseInterface<IJsonResponse>
{
    $AsResponse(IJsonResponse)
public:
    using IHttpResponseInterface::IHttpResponseInterface;
//    using IResponseInterface::operator [];

public:
    IJsonResponse();
    IJsonResponse(const char*);
    IJsonResponse(const QString&);

    IJsonResponse(IJson&&);
    IJsonResponse(const IJson&);

    IJsonResponse(std::string&&);
    IJsonResponse(const std::string&);

    IJsonResponse(IString&&);
    IJsonResponse(const IString&);

    IJsonResponse(QByteArray&&);
    IJsonResponse(const QByteArray&);

    template<typename T>
    IJsonResponse(const T& value);

public:
    virtual std::string prefixMatcher() final;
};

IJsonResponse operator"" _json(const char* str, size_t size);

template<typename T>
IJsonResponse::IJsonResponse(const T& value)
{
    if constexpr (std::is_base_of<IHttpInvalidWare, T>::value){
        this->m_raw->m_status = value.status;
        this->m_raw->setContent(value);
    }else {
        m_raw->setMime(IHttpMime::APPLICATION_JSON_UTF8);
        m_raw->setContent(new IHttpResponseContent(IJsonUtil::toJson(value).dump()));
    }
}

$PackageWebCoreEnd
