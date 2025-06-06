#pragma once

#include "core/util/IHeaderUtil.h"
#include "http/action/IHttpActionMappingInterface.h"
#include "http/assets/IHttpAssetsWare.h"

$PackageWebCoreBegin

class IHttpAssetsWare;
class IHttpAssetsActionMapping : public IHttpActionMappingInterface<IHttpAssetsActionMapping>
{
public:
    IHttpAssetsActionMapping();

public:
    void registAssetsWare(IHttpAssetsWare*);

public:
    virtual void travelPrint() const final;
    virtual IHttpActionWare* getAction(IRequest &request) const;

public:
    virtual double $order() const final;

public:
    std::vector<IHttpAssetsWare*> m_assetsWares;
};

$PackageWebCoreEnd
