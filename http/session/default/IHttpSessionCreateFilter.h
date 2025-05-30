#pragma once

#include "http/filter/IHttpHeaderFilterInterface.h"

$PackageWebCoreBegin

/**
 * @brief 这个类用于提前生成 session
 */
class IHttpSessionCreateFilter : public IHttpHeaderFilterInterface<IHttpSessionCreateFilter, true>
{
public:
    IHttpSessionCreateFilter() = default;

public:
    virtual void filter(IRequest&) final;
};

$PackageWebCoreEnd
