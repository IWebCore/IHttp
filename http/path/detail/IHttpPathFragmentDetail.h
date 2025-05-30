#pragma once

#include "http/path/IHttpPathFragment.h"

$PackageWebCoreBegin

struct IHttpPathFragmentDetail : public IHttpPathFragment
{
public:
    IHttpPathFragmentDetail(const QString& m_fragment);

private:
    bool evaluatePlainText();
    bool evaluateTypeEmptyNode();
    bool evaluateNameOnlyNode();
    bool evaluateNameTypeNode();
    bool evaluateRegTypeNode();
};

$PackageWebCoreEnd
