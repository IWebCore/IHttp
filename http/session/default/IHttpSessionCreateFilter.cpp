#include "IHttpSessionCreateFilter.h"
#include "core/config/IProfileImport.h"
#include "http/IHttpCookieJar.h"
#include "http/IRequest.h"
#include "http/IHttpConstant.h"

$PackageWebCoreBegin

void IHttpSessionCreateFilter::filter(IRequest &request)
{
    static $Bool enabled{"/http/session/enabled", false};
    static $Bool isAutoCreate{"/http/session/autoCreate", false};
    if(! *enabled){
        return;
    }
    if(*isAutoCreate ){
        request.session();
    }else if(request.cookieJar().containRequestCookieKey(IHttp::SESSION_HEADER)){
        request.session();
    }
}

$PackageWebCoreEnd
