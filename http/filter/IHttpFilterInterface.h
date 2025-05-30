#pragma once

#include "core/unit/ISoloUnit.h"
#include "core/task/unit/ITaskWareUnit.h"
#include "http/IHttpTaskCatagory.h"
#include "http/IHttpManage.h"
#include "http/filter/IHttpFilterWare.h"

$PackageWebCoreBegin

template<typename T, IHttpFilterWare::Type filterType, bool enabled>
class IHttpFilterInterface : public IHttpFilterWare,
        public ITaskWareUnit<T, IHttpTaskCatagory, enabled>, public ISoloUnit<T>
{
public:
    virtual void $task() final;
    virtual Type type() const final;
};

template<typename T, IHttpFilterWare::Type filterType, bool enabled>
void IHttpFilterInterface<T, filterType, enabled>::$task()
{
    if (enabled){
        IHttpManage::instance().registerFilter(&ISolo<T>());
    }
}

template<typename T, IHttpFilterWare::Type filterType, bool enabled>
IHttpFilterWare::Type IHttpFilterInterface<T, filterType, enabled>::type() const
{
    return filterType;
}

$PackageWebCoreEnd
