#pragma once

#include "core/util/IHeaderUtil.h"
#include "http/invalid/IHttpInvalidInterface.h"

$PackageWebCoreBegin

class IHttpNotFoundInvalid : public IHttpInvalidInterface<IHttpNotFoundInvalid>
{
public:
    IHttpNotFoundInvalid();
    IHttpNotFoundInvalid(const std::string& description);

public:
    virtual void process(const IHttpInvalidWare&, IHttpResponseRaw&) final;
};

$PackageWebCoreEnd
