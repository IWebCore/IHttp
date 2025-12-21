#include "IHttpInvalidRequestAction.h"
#include "http/IRequest.h"

$PackageWebCoreBegin

template class IHttpActionInterface<IHttpInvalidRequestAction>;

void IHttpInvalidRequestAction::invoke(IRequest &request) const
{
    request.startWrite();
}

$PackageWebCoreEnd
