#include "IHttpInternalErrorInvalid.h"

$PackageWebCoreBegin

IHttpInternalErrorInvalid::IHttpInternalErrorInvalid()
    : IHttpInvalidInterface(IHttpStatus::INTERNAL_SERVER_ERROR_500)
{
}

IHttpInternalErrorInvalid::IHttpInternalErrorInvalid(const char *description)
    : IHttpInternalErrorInvalid(std::string(description))
{
}

IHttpInternalErrorInvalid::IHttpInternalErrorInvalid(const QString &description)
    : IHttpInternalErrorInvalid(description.toStdString())
{
}

IHttpInternalErrorInvalid::IHttpInternalErrorInvalid(const std::string &description)
    : IHttpInvalidInterface(IHttpStatus::INTERNAL_SERVER_ERROR_500, description)
{
}

$PackageWebCoreEnd
