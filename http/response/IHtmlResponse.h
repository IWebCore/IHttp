#pragma once

#include "IHttpResponseInterface.h"
#include "IHttpResponsePreProcessor.h"

$PackageWebCoreBegin

class IHtmlResponse : public IHttpResponseInterface<IHtmlResponse>
{
    $AsResponse(IHtmlResponse)
public:
    using IHttpResponseInterface::IHttpResponseInterface;
    using IHttpResponseInterface::operator [];
    using IHttpResponseInterface::setContent;

public:
    IHtmlResponse();
    IHtmlResponse(const char*);
    IHtmlResponse(const QString& data);

    IHtmlResponse(std::string&&);
    IHtmlResponse(const std::string&);

    IHtmlResponse(QByteArray&&);
    IHtmlResponse(const QByteArray&);

    IHtmlResponse(IString&&);
    IHtmlResponse(const IString&);

    void setContent(IString&&);
    void setContent(const IString&);

public:
    virtual std::string prefixMatcher() final;
};

IHtmlResponse operator"" _html(const char* str, size_t size);

$PackageWebCoreEnd
