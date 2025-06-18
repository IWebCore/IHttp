#pragma once

#include "http/biscuits/IHttpMethod.h"
#include "http/action/IHttpActionInterface.h"
#include "http/callable/IHttpCallable.h"
#include "http/path/IHttpPath.h"
#include "http/path/IHttpPathFragment.h"

$PackageWebCoreBegin

struct IHttpControllerAction : public IHttpActionInterface<IHttpControllerAction>
{
private:
    using ParamType = std::array<void*, 11>;

public:
    virtual void invoke(IRequest &req) const final;

public:
    void setProperty(const QString&key, const QString& value);

private:
    ParamType createParams(IRequest&) const;
    void destroyParams(const ParamType&) const;

public:
    IHttpPath m_path;
    IHttpMethod m_httpMethod;
    IHttpCallable m_callable;
    QMap<QString, QStringList>* m_properties{};
};

$PackageWebCoreEnd
