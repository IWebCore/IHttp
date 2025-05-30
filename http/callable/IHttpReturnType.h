#pragma once

#include "core/util/IHeaderUtil.h"
#include <functional>

$PackageWebCoreBegin

class IHttpRequestImpl;
struct IHttpReturnType
{
public:
    void* create() const;
    void destroy(void*ptr) const;

public:
    QMetaType::Type m_typeId{QMetaType::UnknownType};
    std::string m_typeName;
    std::function<void(IHttpRequestImpl&, void*)> m_resolveFunction {nullptr};
};

$PackageWebCoreEnd
