#pragma once

#include "http/IHttpMultiPart.h"
#include "http/biscuits/IHttpMime.h"
#include "http/biscuits/IHttpMethod.h"
#include "http/biscuits/IHttpVersion.h"
#include "tcp/ITcpResolver.h"

$PackageWebCoreBegin

class IHttpCookieJar;
class IHttpSession;
class IHttpHeaderJar;
class IHttpMultiPartJar;
class IHttpRequestImpl;
class IHttpInvalidWare;
class IRequest : public ITcpResolver
{
    friend class IResponse;
    using ISocket = asio::ip::tcp::socket;
public:
    IRequest() = delete;
    IRequest(const IRequest &) = delete;
    IRequest &operator=(const IRequest &) = delete;
    IRequest(IRequest&&) = delete;
    IRequest& operator=(IRequest&&) = delete;

    explicit IRequest(ITcpConnection& connection, int resolveFactoryId);
    ~IRequest();

public:
    IStringView operator[](const char* header) const;
    IStringView operator[](const std::string& header) const;
    IStringView operator[](const IString& header) const;
    IStringView operator[](const QString& header) const;

public:
    bool isSessionExist() const;
    const IHttpCookieJar& cookieJar() const;
    IHttpSession& session();
    const IHttpHeaderJar& headerJar() const;
    const IHttpMultiPartJar& multiPartJar() const;
    IHttpRequestImpl& impl() const;

public:
    IStringView url() const;
    IHttpVersion version() const;
    IHttpMime mime() const;
    IHttpMethod method() const;

    std::size_t contentLength() const;
    IStringView contentType() const;
    IStringView content() const;

    const QMap<IStringView, IStringView>& pathParameters() const;
    const QMap<IStringView, IStringView>& queryParameters() const;
    const QMap<IStringView, IStringView>& bodyFormParameters() const;
    const std::vector<IHttpMultiPart>& bodyMultiParts() const;
    const IJson& bodyJson() const;

public:
    bool isValid() const;
    void setInvalid(const IHttpInvalidWare&) const;

public:
    virtual void startRead() final;
    virtual void resolve() final;
    virtual void startWrite() final;

private:
    virtual std::vector<asio::const_buffer> getOutput() final;

public:
    IStringView stash(const char *data) const;
    IStringView stash(const QString& data) const;

    IStringView stash(std::string&& data) const;
    IStringView stash(const std::string& data) const;

    IStringView stash(QByteArray&& data) const;
    IStringView stash(const QByteArray& data) const;

    IStringView stash(IString&& data) const;
    IStringView stash(const IString& data) const;

    IStringView stash(IStringView data) const;

private:
    IHttpRequestImpl* m_impl;
};

$PackageWebCoreEnd
