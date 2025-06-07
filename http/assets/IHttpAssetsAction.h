#pragma once

#include "http/action/IHttpActionInterface.h"

$PackageWebCoreBegin

// heap only
class IHttpAssetsAction : public IHttpActionInterface<IHttpAssetsAction>
{
public:
    IHttpAssetsAction(const QString& path);

public:
    virtual void invoke(IRequest &request) const;

private:
    QString m_path;
};

$PackageWebCoreEnd
