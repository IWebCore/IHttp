#pragma once

#include "core/util/IHeaderUtil.h"
#include "http/path/IHttpPathFragment.h"

$PackageWebCoreBegin

struct IHttpPath
{
public:
    QString m_path;
    std::vector<IHttpPathFragment> m_fragments;
    QMap<IString, int> m_actionNameMap;
};

$PackageWebCoreEnd
