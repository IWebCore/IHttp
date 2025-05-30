#pragma once
#include "core/util/IHeaderUtil.h"
#include "IHttpArgumentType.h"
#include "IHttpReturnType.h"

$PackageWebCoreBegin

struct IHttpCallable
{
    void* m_handler{nullptr};
    QString m_className;
    QMetaMethod m_metaMethod;

    QString m_signature;
    IHttpReturnType m_returnNode;
    QVector<IHttpArgumentType> m_argumentNodes;
};

$PackageWebCoreEnd
