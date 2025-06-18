#pragma once

#include "IHttpResponseContent.h"

$PackageWebCoreBegin

class IHttpFileResponseContent : public IHttpResponseContent
{
public:
    IHttpFileResponseContent(IString&&);
    IHttpFileResponseContent(const IString&);

public:
    inline static const IString ContentDispoistion = "ContentDispoistion";
    inline static const IString ContentTypeEnabled = "ContentTypeEnabled"; // find content type automaticly by suffix, default on.
};

$PackageWebCoreEnd
