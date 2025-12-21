#pragma once

#include "IHttpResponseInterface.h"
#include "IHttpResponsePreProcessor.h"

$PackageWebCoreBegin

class IBytesResponse : public IHttpResponseInterface<IBytesResponse>
{
    $AsResponse(IBytesResponse)
public:
    using IHttpResponseInterface::IHttpResponseInterface;
    using IHttpResponseInterface::operator [];
    using IHttpResponseInterface::setContent;

public:
    IBytesResponse();
    IBytesResponse(const char* data);
    IBytesResponse(const QString& data);

    IBytesResponse(QByteArray &&data);
    IBytesResponse(const QByteArray &data);

    IBytesResponse(std::string&&);
    IBytesResponse(const std::string&);

    IBytesResponse(IString&&);
    IBytesResponse(const IString&);

public:
    virtual std::string prefixMatcher() final;
};

IBytesResponse operator"" _bytes(const char* str, size_t size);

extern template class IHttpResponseInterface<IBytesResponse>;

$PackageWebCoreEnd
