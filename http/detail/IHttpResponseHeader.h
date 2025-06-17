#pragma once

#include "core/util/IHeaderUtil.h"

$PackageWebCoreBegin

class IHttpResponseRaw;
class IHttpResponseHeader
{
public:
    explicit IHttpResponseHeader(IHttpResponseRaw& raw, const IString& key);

public:
    operator IStringView() noexcept;
    const IHttpResponseHeader& operator=(const IString& value);
    IStringView key();
    IStringView value();

private:
    IHttpResponseRaw& m_raw;
    IString m_key;
};

$PackageWebCoreEnd
