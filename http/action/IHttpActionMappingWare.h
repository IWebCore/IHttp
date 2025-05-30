#pragma once

#include "core/util/IHeaderUtil.h"

$PackageWebCoreBegin

class IRequest;
class IHttpActionWare;
class IHttpActionMappingWare
{
public:
    IHttpActionMappingWare() = default;
    virtual ~IHttpActionMappingWare() = default;

public:
    virtual void travelPrint() const = 0;
    virtual IHttpActionWare* getAction(IRequest& request) const = 0;
};

$PackageWebCoreEnd
