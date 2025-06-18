#pragma once

#include "http/biscuits/IHttpStatus.h"
#include "http/biscuits/IHttpMime.h"

$PackageWebCoreBegin

class IHttpResponseRaw;
class IHttpInvalidWare
{
    friend class IHttpInvalidReponseContent;
public:
    IHttpInvalidWare(IHttpStatus);
    IHttpInvalidWare(IHttpStatus, const std::string& description);
    virtual ~IHttpInvalidWare() = default;

public:
    virtual void process(const IHttpInvalidWare&, IHttpResponseRaw&) const;

public:
    IHttpStatus status{IHttpStatus::UNKNOWN};
    IHttpMime mime{IHttpMime::TEXT_PLAIN_UTF8};
    IString description;

protected:
    std::function<void(const IHttpInvalidWare&, IHttpResponseRaw&)> m_processor{nullptr};
};

$PackageWebCoreEnd
