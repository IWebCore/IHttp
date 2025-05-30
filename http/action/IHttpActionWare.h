#pragma once

#include "core/util/IHeaderUtil.h"

$PackageWebCoreBegin

class IRequest;
class IHttpActionWare
{
public:
    IHttpActionWare() = default;
    virtual ~IHttpActionWare() = default;

public:
    virtual void invoke(IRequest& request) const = 0;
};

$PackageWebCoreEnd
