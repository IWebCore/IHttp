#include "IHttpHeader.h"
#include "http/IHttpConstant.h"

$PackageWebCoreBegin

void IHttpHeader::insert(IString key, IString value)
{
    m_header[std::move(key)] = std::move(value);
}

void IHttpHeader::remove(const IString& key)
{
    m_header.erase(key);
}

IStringViewList IHttpHeader::keys() const
{
    IStringViewList ret;
    for(const auto& pair : m_header){
        ret.push_back(pair.first);
    }
    return ret;
}

IStringView IHttpHeader::value(const IString& key) const
{
    for(const auto& pair : m_header){
        if(pair.first == key){
            return pair.second;
        }
    }
    return IHttp::EMPTY;
}

bool IHttpHeader::contain(const IString& key) const
{
    return m_header.find(key) != m_header.end();
}

bool IHttpHeader::isEmpty() const
{
    return m_header.empty();
}

$PackageWebCoreEnd


