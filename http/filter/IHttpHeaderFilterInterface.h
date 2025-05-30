#pragma once

#include "http/filter/IHttpFilterInterface.h"

$PackageWebCoreBegin

template<typename T, bool enabled=true>
class IHttpHeaderFilterInterface
    : public IHttpFilterInterface<T, IHttpFilterWare::Type::Header, enabled>
{

};

$PackageWebCoreEnd
