#pragma once

#include "http/filter/IHttpFilterInterface.h"

$PackageWebCoreBegin

template<typename T, bool enabled=true>
class IHttpBodyFilterInterface
    : public IHttpFilterInterface<T, IHttpFilterWare::Type::Body, enabled>
{

};

$PackageWebCoreEnd
