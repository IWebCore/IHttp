#pragma once

#include "core/util/IHeaderUtil.h"
#include "core/task/unit/ITaskWareUnit.h"
#include "core/unit/ISoloUnit.h"
#include "http/action/IHttpActionMappingWare.h"
#include "http/IHttpTaskCatagory.h"
#include "http/IHttpManage.h"

$PackageWebCoreBegin

template<typename T, bool enabled = true>
class IHttpActionMappingInterface : public IHttpActionMappingWare, public ITaskWareUnit<T, IHttpTaskCatagory, enabled>, public ISoloUnit<T>
{
public:
    IHttpActionMappingInterface() = default;

public:
    virtual void $task() final;
};

template<typename T, bool enabled>
void IHttpActionMappingInterface<T, enabled>::$task()
{
    if constexpr (enabled){
        IHttpManage::instance().registActionMapping(&T::instance());
    }
}

$PackageWebCoreEnd
