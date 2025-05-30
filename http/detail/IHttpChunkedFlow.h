#pragma once

#include "core/util/IHeaderUtil.h"

$PackageWebCoreBegin

struct IHttpChunkedFlow
{
public:
    std::size_t m_chunkedPos{};
    bool m_isChunkedBodyLoaded{false};
    std::list<std::pair<std::size_t, std::size_t>> m_chunkedDataStash{};
};

$PackageWebCoreEnd
