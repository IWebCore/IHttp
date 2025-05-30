#pragma once

#include "core/util/IHeaderUtil.h"
#include "http/action/IHttpActionMappingInterface.h"
#include "http/controller/IHttpControllerNode.h"

$PackageWebCoreBegin

class IHttpControllerActionMapping : public IHttpActionMappingInterface<IHttpControllerActionMapping>
{
public:
    IHttpControllerActionMapping() = default;

public:
    void registerUrlActionNode(const IHttpControllerAction& node);

public:
    virtual void travelPrint() const final;
    virtual IHttpActionWare* getAction(IRequest &request) const;

private:
    std::vector<IHttpActionWare*> queryFunctionNodes(const IHttpControllerNode* parentNode, const IStringViewList& fragments, IHttpMethod method) const;

private:
    IHttpControllerNode m_urlMapppings;
};

$PackageWebCoreEnd
