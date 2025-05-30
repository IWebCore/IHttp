#pragma once

#include "core/util/IHeaderUtil.h"
#include "core/unit/ISoloUnit.h"

$PackageWebCoreBegin

class IHttpSessionWare;
class IHttpSessionManager : public QObject, public ISoloUnit<IHttpSessionManager>
{
public:
    IHttpSessionManager() = default;

public:
    void registerSessionWare(IHttpSessionWare*);
    IHttpSessionWare& getSessionWare();

private:
    IHttpSessionWare* m_sessionWare{};
};

$PackageWebCoreEnd
