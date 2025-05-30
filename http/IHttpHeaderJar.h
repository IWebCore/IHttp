#pragma once

#include "core/util/IHeaderUtil.h"
#include "http/IHttpJarUnit.h"
#include "http/biscuits/IHttpHeader.h"

$PackageWebCoreBegin

class IHttpHeaderJar : public IHttpJarUnit
{
public:
    using IHttpJarUnit::IHttpJarUnit;

public:
    std::vector<IStringView> requestHeaderKeys() const;
    bool containRequestHeaderKey(const IString& key) const;

    IStringView getRequestHeaderValue(const IString& key) const;

    IHttpHeader& responseHeaders();

    std::vector<IStringView> responseHeaderKeys() const;
    bool containResponseHeaderKey(IStringView key) const;
    void addResponseHeader(IString key, IString value);
    void deleteReponseHeader(IStringView key);
};

$PackageWebCoreEnd
