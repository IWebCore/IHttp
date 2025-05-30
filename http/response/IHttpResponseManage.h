#pragma once

#include "core/util/IHeaderUtil.h"
#include "core/unit/ISoloUnit.h"

$PackageWebCoreBegin

class IHttpResponseWare;
//class IResponseTemplateRenderer;
class IHttpResponseManage : public ISoloUnit<IHttpResponseManage>
{
public:
    IHttpResponseManage() = default;

public:
    void registerResponse(const std::string& name, IHttpResponseWare* response);
    bool containResponse(const std::string& name);
    IHttpResponseWare* convertableMatch(const std::string&);

private:
    QMap<std::string, IHttpResponseWare*> m_responses;
    QMap<std::string, IHttpResponseWare*> m_convertableResponses;
};

$PackageWebCoreEnd
