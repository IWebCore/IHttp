#pragma once

#include "core/util/IHeaderUtil.h"

$PackageWebCoreBegin

// TODO: 补充 operator=
class IHttpResponseRaw;
class IResponseHeader
{
public:
    IResponseHeader(IHttpResponseRaw& raw, const IString& key);

public:
    operator IStringView() noexcept;
//    const IResponseHeader& operator=(const IString& value);
    IStringView key();
    IStringView value();

private:
    IHttpResponseRaw& m_raw;
    IString m_key;
};

$PackageWebCoreEnd
