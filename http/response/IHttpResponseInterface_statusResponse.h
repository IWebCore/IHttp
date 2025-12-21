//#pragma once

//#include <QtCore>
//#include "IResponseInterface.h"
#include "http/biscuits/IHttpStatus.h"
#include "IHttpResponsePreProcessor.h"

$PackageWebCoreBegin

class IStatusResponse : public IHttpResponseInterface<IStatusResponse>
{
    $AsResponse(IStatusResponse)
public:
    using IHttpResponseInterface::IHttpResponseInterface;
    using IHttpResponseInterface::operator [];

private:
    using IHttpResponseInterface::setContent;

public:
    IStatusResponse() = default;
    IStatusResponse(const QString&);
    IStatusResponse(const std::string&);
    IStatusResponse(int code, const QString& errorMsg="");
    IStatusResponse(IHttpStatus status, const QString& errorMsg="");

public:
    virtual std::string prefixMatcher() final;
};

IStatusResponse operator"" _status(unsigned long long int);

extern template class IHttpResponseInterface<IStatusResponse>;

$PackageWebCoreEnd
