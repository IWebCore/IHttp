#pragma once

#include "core/util/IHeaderUtil.h"
#include "http/action/IHttpActionWare.h"

$PackageWebCoreBegin

class IRequest;
class IHttpAssetsWare
{
public:
    IHttpAssetsWare() = default;

public:
    virtual void travelPrint() const = 0;
    virtual IHttpActionWare* getAction(IRequest&) const = 0;
};

$PackageWebCoreEnd
