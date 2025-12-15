#include "IHttpServer.h"
#include "core/application/asio/IAsioContext.h"
#include "core/config/IProfileImport.h"
#include "http/detail/task/IHttpResolverFactory.h"

$PackageWebCoreBegin

IHttpServer::IHttpServer()
    : ITcpServer(IAsioContext::instance().getContext())
{
    $StdString ip{"/http/ip"};
    if(!ip.value().empty()){
        m_ip = *ip;
    }

    $Int port{"/http/port", -1};
    if(*port != -1){
        m_port = *port;
    }

    const auto& name = ISolo<IHttpResolverFactory>().getName();
    m_resolverFactoryId = ITcpManage::instance().getResolverFactoryId(name);
}

$PackageWebCoreEnd
