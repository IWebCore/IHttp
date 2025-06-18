#pragma once

#include "core/util/IHeaderUtil.h"

$PackageWebCoreBegin

class IHttpResponseRaw;
class IHttpResponseContent
{
    friend class IHttpResponseRaw;
public:
    IHttpResponseContent() = default;
    IHttpResponseContent(IHttpResponseContent&&) = default;
    IHttpResponseContent& operator=(IHttpResponseContent&&) = default;
    IHttpResponseContent(IString&&);
    IHttpResponseContent(const IString&);
    IHttpResponseContent(const IHttpResponseContent&);
    IHttpResponseContent& operator=(const IHttpResponseContent&);

    virtual ~IHttpResponseContent() = default;

public:
    void setAttribute(IString, IString);

public:
    IString m_content{};
    std::function<void(const IHttpResponseContent&, IHttpResponseRaw&)> m_processor{nullptr};
    QMap<IString, IString>* m_attribute{nullptr};
};

$PackageWebCoreEnd
