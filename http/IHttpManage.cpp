#include "IHttpManage.h"
#include "core/abort/IGlobalAbort.h"
#include "core/unit/ISoloUnit.h"
#include "http/IRequest.h"
#include "http/action/IHttpActionWare.h"
#include "http/action/IHttpActionMappingWare.h"
#include "http/action/IHttpNotFoundAction.h"
#include "http/action/IHttpBadRequestAction.h"
#include "http/action/IHttpOptionsMethodAction.h"
#include "http/action/IHttpInvalidRequestAction.h"
#include "http/invalid/IHttpInvalidHandlerInterface.h"

$PackageWebCoreBegin

void IHttpManage::setIsServerStarted(bool value)
{
    m_isServerStarted = value;
}

IHttpActionWare *IHttpManage::getAction(IRequest &request)
{
    if(request.isValid()){
        if(request.method() == IHttpMethod::OPTIONS){
            return &ISolo<IHttpOptionsMethodAction>();
        }
        for(IHttpActionMappingWare* ware : m_ActionMappings){
            auto action = ware->getAction(request);
            if(action != nullptr){
                return action;
            }
        }
        return &ISolo<IHttpNotFoundAction>();
    }
    return &ISolo<IHttpInvalidRequestAction>();
}

IHttpInvalidHandlerWare *IHttpManage::getInvalidHandler(const QString& name) const
{
    if(m_invalidHandlers.contains(name)){
        return m_invalidHandlers[name];
    }
    return nullptr;
}

void IHttpManage::registActionMapping(IHttpActionMappingWare *ware)
{
    m_ActionMappings.push_back(ware);
}

void IHttpManage::registInvalidHandler(const QString& name, IHttpInvalidHandlerWare * ware)
{
    if(m_invalidHandlers.contains(name)){
        qFatal("error");
    }
    m_invalidHandlers[name] = ware;
}

void IHttpManage::registerPathValidator(const QString &name, ValidatorFun fun)
{
    if(m_pathFunValidators.contains(name)){
        auto info = name + "IHttpManage::registerPathValidator, path validator already registered";
        IGlobalAbort::abortDuplicatedKey(info, $ISourceLocation);
    }

    m_pathFunValidators[name] = fun;
}

void IHttpManage::printActionMappingTrace()
{
    for(IHttpActionMappingWare* ware : m_ActionMappings){
        ware->travelPrint();
    }
}

IHttpManage::ValidatorFun IHttpManage::queryPathFunValidator(const QString &path)
{
    if(m_pathFunValidators.contains(path)){
        return m_pathFunValidators[path];
    }
    return nullptr;
}

void IHttpManage::registerFilter(IHttpFilterWare * ware)
{
    m_resolverFilterWare[ware->type()].push_back(ware);
}

std::vector<IHttpFilterWare*> IHttpManage::getFilters(IHttpFilterWare::Type type)
{
    if(m_resolverFilterWare.find(type) == m_resolverFilterWare.end()){
        return {};
    }
    return m_resolverFilterWare[type];
}

void IHttpManage::registerOptionsMethodHandler(IHttpOptionsMethodHandlerWare *ware)
{
    m_optionsMethodHandlers.push_back(ware);
}

const std::vector<IHttpOptionsMethodHandlerWare *> &IHttpManage::getOptionsMethodHandlers() const
{
    return m_optionsMethodHandlers;
}

$PackageWebCoreEnd
