#pragma once

#include "http/filter/IHttpFilterInterface.h"

$PackageWebCoreBegin

template<typename T, bool enabled = true>
class IHttpCompletionFilterInterface
    : public IHttpFilterInterface<T, IHttpFilterWare::Type::completion, enabled>
{

};

$PackageWebCoreEnd
