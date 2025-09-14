#include "IHttpReturnType.h"
#include "http/IHttpAbort.h"

$PackageWebCoreBegin

void *IHttpReturnType::create() const
{
    if(m_typeId != QMetaType::UnknownType){
        return QMetaType(m_typeId).create();
    }

    if(m_typeName == "IHttpStatus"){
        return new int;
    }
    QString tip = "unable to create http callable return type. Type: " + QString::fromStdString(m_typeName);
    IHttpAbort::abortHttpCallableError(tip, $ISourceLocation);
    return nullptr;
}

void IHttpReturnType::destroy(void *ptr) const
{
    if(m_typeId != QMetaType::UnknownType){
        return QMetaType(m_typeId).destroy(ptr);
    }

    if(m_typeName == "IHttpStatus"){
        return delete static_cast<int*>(ptr);
    }
    QString tip = "unable to destroy http callable return type. Type: " + QString::fromStdString(m_typeName);
    IHttpAbort::abortHttpCallableError(tip, $ISourceLocation);
}

$PackageWebCoreEnd
