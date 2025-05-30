#pragma once

#include "IHttpActionWare.h"

$PackageWebCoreBegin

template<typename T>
class IHttpActionInterface : public IHttpActionWare
{
public:
    IHttpActionInterface() = default;
};

$PackageWebCoreEnd
