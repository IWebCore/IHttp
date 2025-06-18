#pragma once

#include "http/action/IHttpActionInterface.h"

$PackageWebCoreBegin

class IHttpOptionsMethodAction : public IHttpActionInterface<IHttpOptionsMethodAction>
{
public:
    IHttpOptionsMethodAction() = default;

public:
    virtual void invoke(IRequest &request) const final;

private:
    bool processStarOptions(IRequest&) const;
    bool processUrlOptions(IRequest&) const;
};

$PackageWebCoreEnd
