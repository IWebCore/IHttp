#pragma once

#include "http/filter/IHttpPostHandleFilterInterface.h"

$PackageWebCoreBegin

class IHttpSessionDumpFilter : public IHttpPostHandleFilterInterface<IHttpSessionDumpFilter, true>
{
public:
    IHttpSessionDumpFilter() = default;

public:
    virtual void filter(IRequest& request) final;
};

$PackageWebCoreEnd
