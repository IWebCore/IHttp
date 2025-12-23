#pragma once

#include "core/util/IHeaderUtil.h"
#include "http/invalid/IHttpInvalidInterface.h"

$PackageWebCoreBegin

class IHttpBadRequestInvalid : public IHttpInvalidInterface<IHttpBadRequestInvalid>
{
public:
    IHttpBadRequestInvalid();
    IHttpBadRequestInvalid(const char* description);
    IHttpBadRequestInvalid(const QString& description);
    IHttpBadRequestInvalid(const std::string& description);
    IHttpBadRequestInvalid(const IString& description);
};

extern template class IHttpInvalidInterface<IHttpBadRequestInvalid>;

$PackageWebCoreEnd
