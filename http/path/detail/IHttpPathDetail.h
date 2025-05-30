#pragma once

#include "http/path/IHttpPath.h"

$PackageWebCoreBegin

struct IHttpPathDetail : public IHttpPath
{
public:
    IHttpPathDetail(const QStringList& args);

private:
    void checkMappingUrl();
    void checkMappingUrlErrorCommon(IStringView piece);
    void CheckMappingUrlErrorWildCard(IStringView piece);
    bool isPieceWildCard(IStringView piece);
};

$PackageWebCoreEnd


