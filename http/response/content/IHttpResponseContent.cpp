#include "IHttpResponseContent.h"

$PackageWebCoreBegin



IHttpResponseContent::IHttpResponseContent(IString && data)
    : m_content(std::move(data))
{
}

IHttpResponseContent::IHttpResponseContent(const IString & data)
    : m_content(data)
{
}

IHttpResponseContent::IHttpResponseContent(const IHttpResponseContent &rhs)
    :m_content(rhs.m_content), m_processor(rhs.m_processor)
{
    if(rhs.m_attribute){
        m_attribute = new QMap<IString, IString>;
        *m_attribute = *(rhs.m_attribute);
    }
}

IHttpResponseContent &IHttpResponseContent::operator=(const IHttpResponseContent &rhs)
{
    m_content = rhs.m_content;
    m_processor = rhs.m_processor;
    if(rhs.m_attribute){
        m_attribute = new QMap<IString, IString>;
        *m_attribute = *(rhs.m_attribute);
    }
    return *this;
}

void IHttpResponseContent::setAttribute(IString key, IString value)
{
    if(!m_attribute){
        m_attribute = new QMap<IString, IString>;
    }
    (*m_attribute)[key] = std::move(value);
}

$PackageWebCoreEnd
