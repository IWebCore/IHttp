#pragma once

#include "tcp/ITcpServer.h"
#include "http/IHttpAnnomacro.h"

$PackageWebCoreBegin

class IHttpServer : public ITcpServer
{
public:
    IHttpServer();
};

$PackageWebCoreEnd
