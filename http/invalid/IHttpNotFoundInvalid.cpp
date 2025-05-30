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

void IHttpNotFoundInvalid::process(const IHttpInvalidWare &ware, IHttpResponseRaw & raw)
{
    raw.setContent(new IHttpResponseContent(ware.description.m_view.toStdString()));
}

$PackageWebCoreEnd
