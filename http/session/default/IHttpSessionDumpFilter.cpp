#include "IHttpSessionDumpFilter.h"
#include "http/IRequest.h"
#include "http/IResponse.h"
#include "http/IHttpSession.h"
#include "http/IHttpConstant.h"

$PackageWebCoreBegin

void IHttpSessionDumpFilter::filter(IRequest &request)
{
    if(request.isSessionExist()){
        IResponse(request).addCookie(request.session().toCookie());
    }
}

$PackageWebCoreEnd
