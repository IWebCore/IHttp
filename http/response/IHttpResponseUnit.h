#pragma once

#include "core/util/IMetaUtil.h"
#include "core/unit/ISoloUnit.h"
#include "core/task/ITaskPreProcessor.h"
#include "http/response/IHttpResponseManage.h"
#include <iostream>

$PackageWebCoreBegin

template<typename T, bool enabled = true>
class IHttpResponseUnit
{
    $AsTaskUnit(IHttpResponseUnit)
protected:
    IHttpResponseUnit() = default;
    virtual ~IHttpResponseUnit() = default;
};

$UseTaskUnit(IHttpResponseUnit)
{
    if constexpr (enabled){
        static std::once_flag initRegisterFlag;
        std::call_once(initRegisterFlag, [](){
            IMetaUtil::registMetaType<T>();
            IHttpResponseManage::instance()
                    .registerResponse(IMetaUtil::getBareTypeName<T>(), &ISolo<T>());
        });
    }
}

$PackageWebCoreEnd
