#pragma once

#include "http/filter/IHttpFilterInterface.h"

$PackageWebCoreBegin

template<typename T, bool enabled=true>
class IHttpPreHandleFilterInterface
    : public IHttpFilterInterface<T, IHttpFilterWare::Type::PreHandle, enabled>
{

};

$PackageWebCoreEnd
