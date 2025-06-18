#include "IHttpNotFoundInvalid.h"
#include "http/detail/IHttpResponseRaw.h"

$PackageWebCoreBegin

IHttpNotFoundInvalid::IHttpNotFoundInvalid()
    : IHttpInvalidInterface(IHttpStatus::NOT_FOUND_404)
{
}

IHttpNotFoundInvalid::IHttpNotFoundInvalid(const std::string& description)
    : IHttpInvalidInterface(IHttpStatus::NOT_FOUND_404, description)
{
}

void IHttpNotFoundInvalid::process(const IHttpInvalidWare &ware, IHttpResponseRaw & raw) const
{
    raw.setContent(new IHttpResponseContent(ware.description.toStdString()));
}

$PackageWebCoreEnd
