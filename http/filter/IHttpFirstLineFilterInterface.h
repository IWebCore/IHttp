#pragma once

#include "http/filter/IHttpFilterInterface.h"

$PackageWebCoreBegin

template<typename T, bool enabled=true>
class IHttpFirstLineFilterInterface
    : public IHttpFilterInterface<T, IHttpFilterWare::Type::FirstLine, enabled>
{

};

$PackageWebCoreEnd
