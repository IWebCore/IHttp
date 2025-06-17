#include "IHttpResponseHeader.h"
#include "http/detail/IHttpResponseRaw.h"

$PackageWebCoreBegin

IHttpResponseHeader::IHttpResponseHeader(IHttpResponseRaw& raw, const IString &key)
    : m_raw(raw), m_key(key)
{
    m_key.solidify();
}

IHttpResponseHeader::operator IStringView () noexcept
{
    return value();
}

const IHttpResponseHeader& IHttpResponseHeader::operator=(const IString &value)
{
    m_raw.m_headers.insert(m_key, value);
    return *this;
}

IStringView IHttpResponseHeader::key()
{
    return m_key.m_view;
}

IStringView IHttpResponseHeader::value()
{
    return m_raw.m_headers.value(m_key.m_view);
}

$PackageWebCoreEnd
