#pragma once

#include "core/util/IHeaderUtil.h"
#include "core/task/unit/ITaskWareUnit.h"
#include "http/IHttpTaskCatagory.h"
#include "http/IHttpManage.h"
#include "http/action/IHttpOptionsMethodHandlerWare.h"

$PackageWebCoreBegin

template<typename T, bool enabled>
class IHttpOptionsMethodHandlerInterface
    : public IHttpOptionsMethodHandlerWare,
      public ITaskWareUnit<T, IHttpTaskCatagory, enabled>,
      public ISoloUnit<T>
{
public:
    virtual double $order() const override;
    virtual void $task() final;
};

template<typename T, bool enabled>
double IHttpOptionsMethodHandlerInterface<T, enabled>::$order() const
{
    return 10;
}

template<typename T, bool enabled>
void IHttpOptionsMethodHandlerInterface<T, enabled>::$task()
{
    if /*constexpr*/ (enabled){
        IHttpManage::instance().registerOptionsMethodHandler(&ISolo<T>());
    }
}

$PackageWebCoreEnd
