#pragma once

#include "core/util/IHeaderUtil.h"

$PackageWebCoreBegin

class IRequest;
struct IHttpArgumentType
{
public:
    enum Position{
        Auto, Path, Query, Header, Cookie, Session, Body, Form, Json,
    };

public:
    std::function<void*(IRequest&)> m_createFun{nullptr};
    std::function<void(void*)> m_destroyFun{nullptr};

public:
    QMetaType::Type m_typeId{QMetaType::UnknownType};
    IString m_typeName;
    IString m_name;
    Position m_position{Auto};
    bool m_optional{false};
};

$PackageWebCoreEnd
