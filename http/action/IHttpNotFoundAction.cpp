#include "IHttpNotFoundAction.h"
#include "http/invalid/IHttpNotFoundInvalid.h"
#include "http/IRequest.h"

$PackageWebCoreBegin

template class IHttpActionInterface<IHttpNotFoundAction>;

void IHttpNotFoundAction::invoke(IRequest &request) const
{
    request.setInvalid(IHttpNotFoundInvalid());
    request.startWrite();
}

$PackageWebCoreEnd
