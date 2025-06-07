#pragma once

#include "http/assets/IHttpAssetsInterface.h"

$PackageWebCoreBegin

class IHttpDefaultAssets : public IHttpAssetsInterface<IHttpDefaultAssets>
{
public:
    IHttpDefaultAssets();

public:
    virtual bool isValid() const final;
    virtual void travelPrint() const final;
    virtual IHttpActionWare* getAction(IRequest &) const final;
};

$PackageWebCoreEnd
