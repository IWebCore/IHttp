#include "IHttpControllerAction.h"
#include "http/IRequest.h"
#include "http/IResponse.h"
#include "http/detail/IHttpResponseRaw.h"
#include "http/detail/IHttpRequestRaw.h"
#include "tcp/ITcpConnection.h"
#include <QMetaObject>

$PackageWebCoreBegin

void IHttpControllerAction::invoke(IRequest &request) const
{
    auto params = createParams(request);
    if(request.isValid()){
        auto index = m_callable.m_metaMethod.methodIndex();
        auto obj = static_cast<QObject*>(m_callable.m_handler);
        m_callable.m_metaCall(obj, QMetaObject::InvokeMetaMethod, index, params.data());
        if(request.isValid()){
            m_callable.m_returnNode.m_resolveFunction(request.impl(), params[0]);
        }
    }
    destroyParams(params);

    request.startWrite();
}

IHttpControllerAction::ParamType IHttpControllerAction::createParams(IRequest& request) const
{
    ParamType params{0};
    params[0] = m_callable.m_returnNode.create();
    int len = m_callable.m_argumentNodes.length();
    for(int i=0; i<len; i++){
        params[i+1] = m_callable.m_argumentNodes[i].m_createFun(request);
        if(!request.isValid()){
            break;
        }
    }
    return params;
}

void IHttpControllerAction::destroyParams(const IHttpControllerAction::ParamType& params) const
{
    m_callable.m_returnNode.destroy(params[0]);
    int len = m_callable.m_argumentNodes.length();
    for(int i=0; i<len; i++){
        if(params[i+1] && m_callable.m_argumentNodes[i].m_destroyFun){
            m_callable.m_argumentNodes[i].m_destroyFun(params[i+1]);
        }
    }
}

$PackageWebCoreEnd
