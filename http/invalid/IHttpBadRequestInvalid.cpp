#include "IHttpBadRequestInvalid.h"

$PackageWebCoreBegin

IHttpBadRequestInvalid::IHttpBadRequestInvalid()
    : IHttpInvalidInterface(IHttpStatus::BAD_REQUEST_400)
{
}

IHttpBadRequestInvalid::IHttpBadRequestInvalid(const char *description)
    : IHttpBadRequestInvalid(std::string(description))
{

}

IHttpBadRequestInvalid::IHttpBadRequestInvalid(const QString &description)
    : IHttpBadRequestInvalid(description.toStdString())
{

}

IHttpBadRequestInvalid::IHttpBadRequestInvalid(const std::string &description)
    : IHttpInvalidInterface(IHttpStatus::BAD_REQUEST_400, description)
{
}

IHttpBadRequestInvalid::IHttpBadRequestInvalid(const IString &description)
    : IHttpBadRequestInvalid(description.toStdString())
{

}

$PackageWebCoreEnd
