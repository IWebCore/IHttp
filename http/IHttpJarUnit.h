#pragma once

#include "core/util/IHeaderUtil.h"

$PackageWebCoreBegin

class IHttpRequestImpl;
class IHttpJarUnit
{
public:
    IHttpJarUnit() = delete;
    ~IHttpJarUnit() = default;
    IHttpJarUnit(const IHttpJarUnit&) = delete;
    IHttpJarUnit(IHttpJarUnit&&) = delete;
    IHttpJarUnit& operator =(const IHttpJarUnit&) = delete;
    IHttpJarUnit& operator =(IHttpJarUnit&&) = delete;

    IHttpJarUnit(IHttpRequestImpl& impl) : m_impl(impl){}

protected:
    IHttpRequestImpl& m_impl;
};

$PackageWebCoreEnd
