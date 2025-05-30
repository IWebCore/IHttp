#include "IHttpInvalidWare.h"
#include "core/abort/IGlobalAbort.h"
#include "http/IRequest.h"
#include "http/IResponse.h"
#include "http/detail/IHttpRequestRaw.h"
#include "http/detail/IHttpResponseRaw.h"

$PackageWebCoreBegin

IHttpInvalidWare::IHttpInvalidWare(IHttpStatus code)
    : status(code), description(IHttpStatusUtil::toStringDescription(code).toStdString())
{
}

IHttpInvalidWare::IHttpInvalidWare(IHttpStatus code, const std::string& description)
    : status(code), description(description)
{
}

void IHttpInvalidWare::process(const IHttpInvalidWare&, IHttpResponseRaw&)
{
    IGlobalAbort::abortUnReachableCode($ISourceLocation);
}

$PackageWebCoreEnd
