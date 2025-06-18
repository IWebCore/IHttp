#pragma once

#include "http/filter/IHttpPostHandleFilterInterface.h"

class IHttpCorsFilter : public IHttpPostHandleFilterInterface<IHttpCorsFilter>
{
public:
    IHttpCorsFilter();

public:
    virtual void filter(IRequest &) override final;
};

