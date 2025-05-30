#pragma once

#include "core/util/IHeaderUtil.h"
#include "core/util/IMetaUtil.h"
#include "core/task/unit/ITaskWareUnit.h"
#include "http/session/IHttpSessionWare.h"
#include "http/session/IHttpSessionManager.h"
#include "http/IHttpTaskCatagory.h"

$PackageWebCoreBegin

template<typename T, bool enabled=true>
class IHttpSessionInterface : public IHttpSessionWare, public ITaskWareUnit<T, IHttpTaskCatagory, enabled>
{
public:
    IHttpSessionInterface() = default;

public:
    virtual double $order() const override;
    virtual void $task() final;
};

template<typename T, bool enabled>
double IHttpSessionInterface<T, enabled>::$order() const
{
    return 49;
}

template<typename T, bool enabled>
void IHttpSessionInterface<T, enabled>::$task ()
{
    if (enabled){
        static std::once_flag flag;
        std::call_once(flag, [&](){
            IHttpSessionManager::instance().registerSessionWare (&ISolo<T>());
        });
    }
}

$PackageWebCoreEnd
