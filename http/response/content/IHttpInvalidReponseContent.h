#pragma once

#include "IHttpResponseContent.h"
#include "http/invalid/IHttpInvalidWare.h"

$PackageWebCoreBegin

class IHttpInvalidReponseContent : public IHttpResponseContent
{
public:
    IHttpInvalidReponseContent(const IHttpInvalidWare& ware);
};

$PackageWebCoreEnd
