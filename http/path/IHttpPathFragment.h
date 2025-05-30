#pragma once

#include "core/util/IHeaderUtil.h"

$PackageWebCoreBegin

struct IHttpPathFragment
{
public:
    using Validator = std::function<bool(IStringView)>;
    enum PathType{
        TEXT,
        FUNCTION,
        FULL_MATCH
    };

public:
    bool isMatch(IStringView) const;

public:
    PathType m_type{PathType::FULL_MATCH};
    IString m_fragment;
    IString m_name;
    Validator m_validator{nullptr};
};

$PackageWebCoreEnd
