#pragma once

#include "core/util/IHeaderUtil.h"
#include "core/util/IMetaUtil.h"
#include "core/util/IPreProcessorUtil.h"
#include "http/biscuits/IHttpMime.h"

$PackageWebCoreBegin

class IHttpMultiPartJar;
class IRequest;
class IHttpMultiPart
{
public:
    enum TransferEncoding{      // rfc2046
        BIT_7,
        BIT_8,
        BINARY,
    };

public:
    IHttpMultiPart() = default;
    IHttpMultiPart(IStringView data, IRequest* request);
    IHttpMultiPart(const IHttpMultiPart&) = delete;
    IHttpMultiPart& operator = (const IHttpMultiPart&) = delete;
    IHttpMultiPart(IHttpMultiPart&&);
    IHttpMultiPart& operator = (IHttpMultiPart&&);

public:
    bool isValid() const;

public:
    IStringView m_name;
    IStringView m_fileName;
    IStringView m_charset;
    IStringView m_content;
    IHttpMime m_mime {IHttpMime::UNKNOWN};
    TransferEncoding m_encoding{BIT_7};
    IStringViewList m_headers;

public:
    static const IHttpMultiPart Empty;
};

$PackageWebCoreEnd
