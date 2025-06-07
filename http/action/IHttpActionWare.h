#pragma once

#include "core/util/IHeaderUtil.h"

$PackageWebCoreBegin

class IRequest;
class IHttpActionWare
{
public:
    IHttpActionWare() = default;
    virtual ~IHttpActionWare() = default;

public:
    virtual const QString& actionName() const = 0;
    virtual void invoke(IRequest& request) const = 0;

public:
    bool m_isDeleteble{false};
};

$PackageWebCoreEnd
