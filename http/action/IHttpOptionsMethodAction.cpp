#include "IHttpOptionsMethodAction.h"
#include "http/IRequest.h"
#include "http/action/IHttpOptionsMethodHandlerWare.h"
#include "http/detail/IHttpRequestImpl.h"
#include "http/response/IHttpResponseWare.h"
#include "http/invalid/IHttpNotFoundInvalid.h"
#include "http/IHttpManage.h"
#include "IHttpNotFoundAction.h"
#include "IHttpInvalidRequestAction.h"

$PackageWebCoreBegin

void IHttpOptionsMethodAction::invoke(IRequest &request) const
{
    static const auto& handlers = IHttpManage::instance().getOptionsMethodHandlers();
    for(const auto& handler : handlers){
        if(handler->handle(request)){
            return request.startWrite();
        }
    }

    if(processStarOptions(request)){
        return request.startWrite();
    }
    if(processUrlOptions(request)){
        return request.startWrite();
    }
}

bool IHttpOptionsMethodAction::processStarOptions(IRequest &request) const
{
    static const IString AllowContent = "GET, HEAD, POST, PUT, DELETE, PATCH, OPTIONS";

    if(request.url() != "*"){
        return false;
    }

    IHttpResponseWare ware;
    ware.setHeader(IHttpHeader::Allow, AllowContent);
    request.impl().setResponseWare(std::move(ware));
    return true;
}

bool IHttpOptionsMethodAction::processUrlOptions(IRequest& request) const
{
    static const QList<IHttpMethod> METHODS = {
        IHttpMethod::GET, IHttpMethod::POST, IHttpMethod::PUT,
        IHttpMethod::DELETED, IHttpMethod::HEAD, IHttpMethod::PATCH
    };

    IStringViewList views;
    for(auto method : METHODS){
        request.impl().m_reqRaw.m_method = method;
        auto action = IHttpManage::instance().getAction(request);
        if(action != &ISolo<IHttpNotFoundAction>()
                && action != &ISolo<IHttpInvalidRequestAction>()){
            views.append(IHttpMethodUtil::toString(method));
        }
    }
    request.impl().m_reqRaw.m_method = IHttpMethod::OPTIONS;

    if(views.isEmpty()){
        request.setInvalid(IHttpNotFoundInvalid());
    }else{
        views.append(IHttpMethodUtil::toString(IHttpMethod::OPTIONS));
        auto ret = request.stash(views.join(", "));
        IHttpResponseWare ware;
        ware.setStatus(IHttpStatus::NO_CONTENT_204);
        ware.setHeader(IHttpHeader::Allow, ret);
        request.impl().setResponseWare(ware);
    }
    return true;
}

$PackageWebCoreEnd
