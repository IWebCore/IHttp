#include "IHttpResponseContent.h"

$PackageWebCoreBegin

IHttpResponseContent::~IHttpResponseContent()
{
}

IHttpResponseContent::IHttpResponseContent(IString && data)
    : m_content(std::move(data))
{
}

IHttpResponseContent::IHttpResponseContent(const IString & data)
    : m_content(data)
{
}

void IHttpResponseContent::setAttribute(IString key, IString value)
{
    if(!m_attribute){
        m_attribute = new QMap<IString, IString>;
    }
    (*m_attribute)[key] = std::move(value);
}

$PackageWebCoreEnd
