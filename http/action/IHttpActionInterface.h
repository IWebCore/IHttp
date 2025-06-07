#pragma once

#include "IHttpActionWare.h"
#include "core/util/IMetaUtil.h"

$PackageWebCoreBegin

template<typename T>
class IHttpActionInterface : public IHttpActionWare
{
public:
    IHttpActionInterface() = default;

public:
    virtual const QString& actionName() const;
    static const QString& staticActionName();   // this is used for filter specific action
};

template<typename T>
const QString& IHttpActionInterface<T>::actionName() const
{
    return T::staticActionName();
}

template<typename T>
const QString& IHttpActionInterface<T>::staticActionName()
{
    static auto name = IMetaUtil::getTypeName<T>();
    return name;
}

$PackageWebCoreEnd
