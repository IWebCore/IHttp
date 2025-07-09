#pragma once

#include "core/unit/ISoloUnit.h"
#include "http/IRequest.h"
#include "http/biscuits/IHttpStatus.h"
#include "http/filter/IHttpFilterWare.h"

$PackageWebCoreBegin

class IRequest;
class IHttpActionWare;
class IHttpFilterWare;
class IHttpActionMappingWare;
class IHttpInvalidHandlerWare;
class IHttpOptionsMethodHandlerWare;
class IHttpManage : public ISoloUnit<IHttpManage>
{
    using ValidatorFun = std::function<bool(IStringView)>;
public:
    IHttpManage() = default;

public:
    void setIsServerStarted(bool);

public:
    IHttpActionWare* getAction(IRequest& request);
    IHttpInvalidHandlerWare* getInvalidHandler(const QString& name) const;
    void printActionMappingTrace();

    void registActionMapping(IHttpActionMappingWare* ware);
    void registInvalidHandler(const QString& name, IHttpInvalidHandlerWare* ware);

    void registerPathValidator(const QString& name, ValidatorFun fun);
    ValidatorFun queryPathFunValidator(const QString& path);

    void registerFilter(IHttpFilterWare*);
    std::vector<IHttpFilterWare*> getFilters(IHttpFilterWare::Type type);

    void registerOptionsMethodHandler(IHttpOptionsMethodHandlerWare*);
    const std::vector<IHttpOptionsMethodHandlerWare*>& getOptionsMethodHandlers() const;

    template<IHttpFilterWare::Type type>
    void invokeFilters(IRequest&);

private:
    bool m_isServerStarted{false};
    std::vector<IHttpOptionsMethodHandlerWare*> m_optionsMethodHandlers;
    std::vector<IHttpActionMappingWare*> m_ActionMappings;
    QMap<QString, IHttpInvalidHandlerWare*> m_invalidHandlers;
    QMap<QString, ValidatorFun> m_pathFunValidators;
    QMap<IHttpFilterWare::Type, std::vector<IHttpFilterWare*>> m_resolverFilterWare;
};

template<IHttpFilterWare::Type type>
void IHttpManage::invokeFilters(IRequest& request)
{
    static const auto s_filters = getFilters(type);
    static bool s_filterEnabled = !s_filters.empty();
    if(s_filterEnabled){
        for(auto filter : s_filters){
            filter->filter(request);
            if(!request.isValid()){
                return;
            }
        }
    }
}

$PackageWebCoreEnd
