#pragma once

#include "http/filter/IHttpFilterInterface.h"

$PackageWebCoreBegin

template<typename T, bool enabled = true>
class IHttpPostHandleFilterInterface
    : public IHttpFilterInterface<T, IHttpFilterWare::Type::PostHandle, enabled>
{

};

$PackageWebCoreEnd
