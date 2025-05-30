#pragma once

#include "core/util/IHeaderUtil.h"
#include "core/task/unit/ITaskWareUnit.h"
#include "core/unit/ISoloUnit.h"
#include "http/IHttpTaskCatagory.h"
#include "http/IHttpManage.h"
#include <functional>

$PackageWebCoreBegin

template<typename T, bool enabled=true>
class IHttpPathValidatorInterface : public ITaskWareUnit<T, IHttpTaskCatagory, enabled>, public ISoloUnit<T>
{
public:
    using Validator = std::function<bool(IStringView)>;

public:
    virtual void $task() final;
    virtual double $order() const final;

public:
    virtual QString marker() const  = 0;
    virtual Validator validator() const  = 0;
};

template<typename T, bool enabled>
void IHttpPathValidatorInterface<T, enabled>::$task()
{
    IHttpManage::instance().registerPathValidator(marker(), validator());
}

template<typename T, bool enabled>
double IHttpPathValidatorInterface<T, enabled>::$order() const
{
    return 49;
}

$PackageWebCoreEnd
