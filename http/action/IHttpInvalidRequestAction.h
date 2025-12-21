#pragma once

#include "http/action/IHttpActionInterface.h"

$PackageWebCoreBegin

class IHttpInvalidRequestAction : public IHttpActionInterface<IHttpInvalidRequestAction>
{
public:
    IHttpInvalidRequestAction() = default;

public:
    virtual void invoke(IRequest &request) const final;
};

extern template class IHttpActionInterface<IHttpInvalidRequestAction>;

$PackageWebCoreEnd
