#pragma once

#include "core/util/IHeaderUtil.h"
#include "http/response/IHttpResponseUnit.h"
#include "http/response/IHttpResponseWare.h"
#include "http/invalid/IHttpInvalidWare.h"

$PackageWebCoreBegin

// TODO: flatten this interface

class IStatusResponse;
class IRedirectResponse;
template<typename T, bool enabled=true>
class IHttpResponseInterface : public IHttpResponseWare, IHttpResponseUnit<T, enabled>
{
public:
    using IHttpResponseWare::operator [];

public:
    IHttpResponseInterface() = default;
    virtual ~IHttpResponseInterface() = default;

    IHttpResponseInterface(IRedirectResponse&& response);
    IHttpResponseInterface(IStatusResponse&& response);
    IHttpResponseInterface(const IHttpInvalidWare&);

    IHttpResponseInterface(IHttpResponseInterface &&);
    IHttpResponseInterface(const IHttpResponseInterface&);
    IHttpResponseInterface& operator=(const IHttpResponseInterface&);
    IHttpResponseInterface& operator=(IHttpResponseInterface&&);
};

$PackageWebCoreEnd

#include "http/response/IHttpResponseInterface_statusResponse.h"
#include "http/response/IHttpResponseInterface_redirectResponse.h"

$PackageWebCoreBegin

template<typename T, bool enabled>
IHttpResponseInterface<T, enabled>::IHttpResponseInterface(IRedirectResponse &&response)
{
    std::swap(this->m_raw, response.m_raw);
}

template<typename T, bool enabled>
IHttpResponseInterface<T, enabled>::IHttpResponseInterface(IStatusResponse &&response)
{
    std::swap(this->m_raw, response.m_raw);
}

template<typename T, bool enabled>
IHttpResponseInterface<T, enabled>::IHttpResponseInterface(const IHttpInvalidWare& ware)
{ 
    this->m_raw->m_status = ware.status;
    this->m_raw->setContent(ware);
}

template<typename T, bool enabled>
IHttpResponseInterface<T, enabled>::IHttpResponseInterface(const IHttpResponseInterface &rhs)
    : IHttpResponseWare(rhs)
{
}

template<typename T, bool enabled>
IHttpResponseInterface<T, enabled>::IHttpResponseInterface(IHttpResponseInterface &&rhs)
    : IHttpResponseWare(std::forward<IHttpResponseInterface>(rhs))
{
}

template<typename T, bool enabled>
IHttpResponseInterface<T, enabled> &IHttpResponseInterface<T, enabled>::operator=(const IHttpResponseInterface&rhs)
{
    IHttpResponseWare::operator =(rhs);
    return *this;
}

template<typename T, bool enabled>
IHttpResponseInterface<T, enabled> &IHttpResponseInterface<T, enabled>::operator=(IHttpResponseInterface &&rhs)
{
    IHttpResponseWare::operator =(std::move(rhs));
    return *this;
}

$PackageWebCoreEnd

