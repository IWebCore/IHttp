#pragma once

#include "core/util/IConvertUtil.h"
#include "core/base/IResult.h"
#include "http/biscuits/IHttpHeader.h"
#include "http/biscuits/IHttpVersion.h"
#include "http/biscuits/IHttpMime.h"
#include "http/biscuits/IHttpStatus.h"
#include "http/biscuits/IHttpMethod.h"
#include "http/biscuits/IHttpAttribute.h"
#include "http/action/IHttpActionWare.h"
#include "http/detail/IHttpResponseRaw.h"
#include "http/detail/IHttpRequestRaw.h"
#include "http/IHttpManage.h"
#include "http/IHttpMultiPart.h"
#include "http/IHttpHeaderJar.h"
#include "http/IHttpMultiPartJar.h"
#include "http/IHttpCookieJar.h"
#include "http/IHttpSession.h"
#include "http/response/content/IHttpResponseContent.h"
#include "http/filter/IHttpFilterWare.h"

$PackageWebCoreBegin

class IResponse;
class IRequest;
class ITcpConnection;
struct ITcpConnectionData;
struct IHttpChunkedFlow;
class IHttpRequestImpl : public IStringViewStash
{
    friend class IRequest;
private:
    enum class State{
        FirstLineState, HeaderState, LoadBodyState, BodyState, EndState
    };

public:
    IHttpRequestImpl(IRequest& self);
    ~IHttpRequestImpl();

private:
    void parseData();
    void firstLineState();
    void headersState();
    bool loadBodyState();
    void bodyState();
    void endState();

private:
    bool prepareContentLengthData();
    bool prepareChunkedData();

private:
    void parseFirstLine(IStringView);
    void resolveFirstLine();
    void parseHeaders(IStringView);
    void parseHeader(IStringView);
    void parseCookie(IStringView);
    void resolveHeaders();
    void resolvePayload();
    void parseUrlEncodedData(IStringView data, bool isBody);
    void parseJsonData(IStringView data);
    void parseMultiPartData(IStringView data);
    void parseAction();

private:
    IStringView getBoundary(IStringView);
    bool tryParseChunkedData();
    IStringView montageChunkedData();
    IStringView parseChunkedData(IStringView);

private:
    void resolveHeaderContentDataType(); // content-length or chunked data
    void resolveHeaderContentType();
    void resolveHeaderKeepAlive();

public:
    void setInvalid(const IHttpInvalidWare&);
    void setResponseWare(const IHttpResponseWare& ware);

public:
    bool isSessionExist() const;
    IHttpSession& session();

public:
    bool m_isValid{true};
    IRequest& m_request;

public:
    IHttpRequestRaw m_reqRaw{};
    IHttpResponseRaw m_respRaw{};

public:
    IHttpActionWare* m_action{};
    ITcpConnection& m_connection;
    ITcpConnectionData& m_data;

public:
    IHttpHeaderJar m_headerJar;
    IHttpCookieJar m_cookieJar;
    IHttpMultiPartJar m_multiPartJar;
    IHttpAttribute m_attribute;

private:
    IHttpSession* m_session{nullptr};

private:
    bool m_requestComplete{false};
    State m_state{State::FirstLineState};
    IStringView m_multipartBoundary;
    IStringView m_multipartBoundaryEnd;

private:
    IHttpChunkedFlow* m_chunkFlow{};
};

$PackageWebCoreEnd
