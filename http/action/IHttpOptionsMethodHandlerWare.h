#pragma once

#include "core/util/IHeaderUtil.h"

$PackageWebCoreBegin

class IRequest;
class IHttpOptionsMethodHandlerWare
{
public:
    IHttpOptionsMethodHandlerWare() = default;

public:
    virtual bool handle(IRequest&) const = 0;
};

$PackageWebCoreEnd
