#pragma once

#include "core/util/IHeaderUtil.h"
#include "core/util/IMetaUtil.h"
#include "http/IHttpManage.h"
#include "http/biscuits/IHttpStatus.h"
#include "http/invalid/IHttpInvalidWare.h"
#include "http/invalid/IHttpInvalidHandlerInterface.h"
#include <functional>
#include <string>
#include <iostream>

$PackageWebCoreBegin

template<typename T, bool enabled=true>
class IHttpInvalidInterface : public IHttpInvalidWare
{
public:
    IHttpInvalidInterface(IHttpStatus code);
    IHttpInvalidInterface(IHttpStatus code, const std::string& description);
};

template<typename T, bool enabled>
IHttpInvalidInterface<T, enabled>::IHttpInvalidInterface(IHttpStatus status)
    : IHttpInvalidWare(status, IMetaUtil::getBareTypeName<T>())
{
}

template<typename T, bool enabled>
IHttpInvalidInterface<T, enabled>::IHttpInvalidInterface(IHttpStatus status, const std::string& description)
    : IHttpInvalidWare(status, description)
{
    static std::function<void(const IHttpInvalidWare&, IHttpResponseRaw&)> s_funs {nullptr};
    static std::once_flag flag;
    std::call_once(flag, [&](){
        bool derived = typeid(&T::process) != typeid(&IHttpInvalidWare::process);
        if (derived){
            s_funs  = [](const IHttpInvalidWare& ware, IHttpResponseRaw& respRaw){
                ISolo<T>().T::process(ware, respRaw);
            };
        }
        auto handler = IHttpManage::instance().getInvalidHandler(IMetaUtil::getTypeName<T>());
        if(handler){
            s_funs = [=](const IHttpInvalidWare& ware, IHttpResponseRaw& respRaw) {
                handler->handle(ware, respRaw);
            };
        }
    });
    m_processor = s_funs;
}

$PackageWebCoreEnd
