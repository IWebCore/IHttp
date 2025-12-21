//#pragma once

//#include "IResponseInterface.h"
#include "IHttpResponsePreProcessor.h"
#include "http/biscuits/IHttpStatus.h"

$PackageWebCoreBegin

class IRedirectResponse : public IHttpResponseInterface<IRedirectResponse>
{
    $AsResponse(IRedirectResponse)
public:
    using IHttpResponseInterface::IHttpResponseInterface;
    using IHttpResponseInterface::operator [];

private:
    using IHttpResponseInterface::setContent;

public:
    IRedirectResponse();
    IRedirectResponse(const char* data, IHttpStatus = IHttpStatus::FOUND_302);
    IRedirectResponse(const QString &path, IHttpStatus = IHttpStatus::FOUND_302);
    IRedirectResponse(const std::string& path, IHttpStatus = IHttpStatus::FOUND_302);
    IRedirectResponse(const QByteArray&& path, IHttpStatus = IHttpStatus::FOUND_302);
    IRedirectResponse(const IString&& path, IHttpStatus = IHttpStatus::FOUND_302);

public:
    virtual std::string prefixMatcher() final;
    void updateLocationPath();

private:
    QString m_redirectPath;
};

IRedirectResponse operator"" _redirect(const char* str, size_t size);

extern template class IHttpResponseInterface<IRedirectResponse>;

$PackageWebCoreEnd

