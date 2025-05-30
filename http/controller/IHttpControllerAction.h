#pragma once

#include "http/biscuits/IHttpMethod.h"
#include "http/action/IHttpActionInterface.h"
#include "http/callable/IHttpCallable.h"
#include "http/path/IHttpPath.h"
#include "http/path/IHttpPathFragment.h"

$PackageWebCoreBegin

struct IHttpControllerAction : public IHttpActionInterface<IHttpControllerAction>
{
private:
    using ParamType = std::array<void*, 11>;

public:
    virtual void invoke(IRequest &req) const final;

private:
    ParamType createParams(IRequest&) const;
    void destroyParams(const ParamType&) const;

public:
//    void* m_parentNode{};
    IHttpPath m_path;
    IHttpMethod m_httpMethod;
    IHttpCallable m_callable;
};

$PackageWebCoreEnd
