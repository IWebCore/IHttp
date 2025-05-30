#pragma once

#include "core/util/IHeaderUtil.h"
#include "core/util/IMetaUtil.h"
#include "core/task/unit/ITaskWareUnit.h"
#include "core/unit/ISoloUnit.h"
#include "core/util/ISpawnUtil.h"
#include "http/IHttpTaskCatagory.h"

#include "http/controller/pp/IControllerPreProcessor.h"
#include "http/controller/pp/IDeleteMappingPreProcessor.h"
#include "http/controller/pp/IGetMappingPreProcessor.h"
#include "http/controller/pp/IPatchMappingPreProcessor.h"
#include "http/controller/pp/IPostMappingPreProcessor.h"
#include "http/controller/pp/IPutMappingPreProcessor.h"

#include "http/IRequest.h"
#include "http/IResponse.h"

$PackageWebCoreBegin

template<typename T, bool enabled = true>
class IHttpControllerInterface : public ITaskWareUnit<T, IHttpTaskCatagory, enabled>, public ISoloUnit<T>
{
public:
    virtual void $task() final;
};

template<typename T, bool enabled>
void IHttpControllerInterface<T, enabled>::$task()
{
    if constexpr (enabled){
        auto className = IMetaUtil::getMetaClassName (T::staticMetaObject);
        auto classInfo = IMetaUtil::getMetaClassInfoMap(T::staticMetaObject);
        auto classMethods = IMetaUtil::getMetaMethods(T::staticMetaObject);
        ISpawnUtil::construct<void, void*, const QString&, const QMap<QString, QString> &, const QVector<QMetaMethod>&>
            (this, className, classInfo, classMethods);
    }
}

$PackageWebCoreEnd
