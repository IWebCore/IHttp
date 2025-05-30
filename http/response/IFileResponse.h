#pragma once

#include "IHttpResponseInterface.h"
#include "IHttpResponsePreProcessor.h"

$PackageWebCoreBegin

class IHttpFileResponseContent;
class IFileResponse : public IHttpResponseInterface<IFileResponse>
{
    $AsResponse(IFileResponse)
public:
    using IHttpResponseInterface::IHttpResponseInterface;
//    using IResponseInterface::operator [];

public:
    IFileResponse() = default;
    IFileResponse(const char* data);
    IFileResponse(const QString& path);

    IFileResponse(IString&& path);
    IFileResponse(const IString& path);

    IFileResponse(const std::string& path);
    IFileResponse(std::string&& path);

public:
    void enableContentDisposition();

public:
    virtual std::string prefixMatcher() final;

private:
    IHttpFileResponseContent* m_fileResponseContent{};
};

IFileResponse operator"" _file(const char* str, size_t size);

$PackageWebCoreEnd
