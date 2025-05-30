#include "IHttpMultiPartJar.h"
#include "core/abort/IGlobalAbort.h"
#include "http/detail/IHttpRequestRaw.h"
#include "http/detail/IHttpResponseRaw.h"
#include "http/detail/IHttpRequestImpl.h"

$PackageWebCoreBegin

const IHttpMultiPart& IHttpMultiPartJar::operator[](const QString &name) const
{
    auto temp = name.toUtf8();
    return this->operator [](IStringView(temp));
}

const IHttpMultiPart& IHttpMultiPartJar::operator[](const IString& name) const
{
    const auto& jar = m_impl.m_reqRaw.m_multiParts;
    for(const auto& part : jar){
        if(part.m_name == name){
            return part;
        }
    }
    return IHttpMultiPart::Empty;
}

bool IHttpMultiPartJar::contain(const IString& name) const
{
    const auto& jar = m_impl.m_reqRaw.m_multiParts;
    for(const auto& part : jar){
        if(part.m_name == name){
            return true;
        }
    }
    return false;
}

bool IHttpMultiPartJar::contain(const QString &name) const
{
    return contain(IString(name.toUtf8()));
}

IStringViewList IHttpMultiPartJar::getNames() const
{
    IStringViewList ret;
    const auto& jar = m_impl.m_reqRaw.m_multiParts;
    for(const auto& part : jar){
        ret.append (part.m_name);
    }
    return ret;
}

const IHttpMultiPart& IHttpMultiPartJar::getMultiPart(const IString& name) const
{
    const auto& jar = m_impl.m_reqRaw.m_multiParts;
    for(const IHttpMultiPart& part : jar){
        if(part.m_name == name){
            return part;
        }
    }
    return IHttpMultiPart::Empty;
}

const IHttpMultiPart& IHttpMultiPartJar::getMultiPart(const QString &name) const
{
    return getMultiPart(IString(name.toUtf8()));
}

const std::vector<IHttpMultiPart> &IHttpMultiPartJar::getRequestMultiParts() const
{
    return m_impl.m_reqRaw.m_multiParts;
}

$PackageWebCoreEnd
