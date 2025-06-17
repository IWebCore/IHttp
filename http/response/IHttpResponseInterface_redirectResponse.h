//#pragma once

//#include "IResponseInterface.h"
#include "IHttpResponsePreProcessor.h"

$PackageWebCoreBegin

// TODO: 参数可以多添加一个 code， 让用户选择是哪个返回状态 301， 302， 307， 308
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
    IRedirectResponse(const char* data);
    IRedirectResponse(const QString &path);
    IRedirectResponse(const std::string& path);
    IRedirectResponse(const QByteArray&& path);
    IRedirectResponse(const IString&& path);

public:
    virtual std::string prefixMatcher() final;
    void updateLocationPath();

private:
    QString m_redirectPath;
};

IRedirectResponse operator"" _redirect(const char* str, size_t size);

$PackageWebCoreEnd

