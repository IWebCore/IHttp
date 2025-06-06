#pragma once

#include "core/util/IHeaderUtil.h"
#include "core/task/unit/ITaskWareUnit.h"
#include "core/unit/ISoloUnit.h"
#include "http/IHttpTaskCatagory.h"
#include "http/assets/IHttpAssetsWare.h"
#include "http/assets/IHttpAssetsActionMapping.h"

$PackageWebCoreBegin

template<typename T, bool enabled=true>
class IHttpAssetsInterface : public IHttpAssetsWare, public ITaskWareUnit<T, IHttpTaskCatagory, enabled>, public ISoloUnit<T>
{
public:
    IHttpAssetsInterface() = default;

public:
    virtual bool isValid() const;

public:
    virtual void $task() final;
    virtual double $order() const final;
};

template<typename T, bool enabled>
bool IHttpAssetsInterface<T, enabled>::isValid() const
{
    return true;
}

template<typename T, bool enabled>
void IHttpAssetsInterface<T, enabled>::$task()
{
    if (enabled){
        if(isValid()){
            IHttpAssetsActionMapping::instance().registAssetsWare(&ISolo<T>());
        }else{
            qDebug() << IMetaUtil::getTypeName<T>() << " is not registered due to its invalid";
        }
    }
}

template<typename T, bool enabled>
double IHttpAssetsInterface<T, enabled>::$order() const
{
    return 52;
}

$PackageWebCoreEnd
