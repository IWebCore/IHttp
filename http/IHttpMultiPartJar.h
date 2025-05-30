#pragma once

#include "core/util/IHeaderUtil.h"
#include "core/util/IMetaUtil.h"
#include "core/base/IResult.h"
#include "http/IHttpJarUnit.h"
#include "http/IHttpMultiPart.h"

$PackageWebCoreBegin

class IHttpMultiPartJar : public IHttpJarUnit
{
public:
    using IHttpJarUnit::IHttpJarUnit;

public:
    const IHttpMultiPart& operator[](const IString& name) const;
    const IHttpMultiPart& operator[](const QString& name) const;

    bool contain(const IString& name) const;
    bool contain(const QString& name) const;
    IStringViewList getNames() const;

    const IHttpMultiPart& getMultiPart(const IString& name) const;
    const IHttpMultiPart& getMultiPart(const QString& name) const;
    const std::vector<IHttpMultiPart>& getRequestMultiParts() const;
};

$PackageWebCoreEnd
