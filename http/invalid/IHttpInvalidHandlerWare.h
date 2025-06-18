#pragma once

#include "core/task/unit/ITaskWareUnit.h"

$PackageWebCoreBegin

class IHttpResponseRaw;
class IHttpInvalidWare;
class IHttpInvalidHandlerWare
{
public:
    IHttpInvalidHandlerWare() = default;

public:
    virtual void handle(const IHttpInvalidWare&, IHttpResponseRaw&) const;
};

$PackageWebCoreEnd
