#pragma once
#include "core/util/IHeaderUtil.h"
#include "IHttpResponseInterface.h"
#include "IHttpResponsePreProcessor.h"
#include "core/util/IConvertUtil.h"

$PackageWebCoreBegin

class IPlainTextResponse : public IHttpResponseInterface<IPlainTextResponse>
{
    $AsResponse(IPlainTextResponse)
public:
    using IHttpResponseInterface::IHttpResponseInterface;
//    using IResponseInterface::operator [];

public:
    IPlainTextResponse();
    virtual ~IPlainTextResponse() = default;

    IPlainTextResponse(const char* value);
    IPlainTextResponse(const QString& value);

    IPlainTextResponse(std::string&& value);
    IPlainTextResponse(const std::string& value);

    IPlainTextResponse(QByteArray&& value);
    IPlainTextResponse(const QByteArray& value);

    IPlainTextResponse(IString&& value);
    IPlainTextResponse(const IString& value);

public:
    virtual std::string prefixMatcher() final;
};

IPlainTextResponse operator"" _text(const char* str, size_t size);

$PackageWebCoreEnd


