#pragma once

#include "core/util/IHeaderUtil.h"
#include "http/action/IHttpActionWare.h"

$PackageWebCoreBegin

class IRequest;
class IHttpAssetsWare
{
public:
    IHttpAssetsWare();

public:
    virtual IHttpActionWare* getAction(IRequest&) const = 0;
};

$PackageWebCoreEnd
