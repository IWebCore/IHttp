#include "IHttpRequestImpl.h"

#include "core/util/IHeaderUtil.h"
#include "core/config/IProfileImport.h"
#include "http/IRequest.h"
#include "http/IHttpManage.h"
#include "http/IHttpConstant.h"
#include "http/IHttpSession.h"
#include "http/action/IHttpActionWare.h"
#include "http/controller/IHttpControllerAction.h"
#include "http/detail/IHttpRequestRaw.h"
#include "http/invalid/IHttpBadRequestInvalid.h"
#include "http/invalid/IHttpNotFoundInvalid.h"
#include "http/invalid/IHttpInternalErrorInvalid.h"
#include "http/response/IHttpResponseWare.h"
#include "http/detail/IHttpChunkedFlow.h"
#include "tcp/ITcpConnection.h"
#include <algorithm>
#include <regex>
#include <utility>

$PackageWebCoreBegin

IHttpRequestImpl::IHttpRequestImpl(IRequest& self)
    : m_request(self),
    m_connection(self.m_connection),
    m_data(self.m_data),
    m_headerJar{ *this },
    m_cookieJar{ *this },
    m_multiPartJar{ *this }
{
}

IHttpRequestImpl::~IHttpRequestImpl()
{
    if(m_session){
        delete m_session;
    }
    if(m_chunkFlow){
        delete m_chunkFlow;
    }
    if(m_action && m_action->m_isDeleteble){
        delete m_action;
    }
}

void IHttpRequestImpl::parseData()
{
    while (true) {
        switch (m_state) {
        case State::FirstLineState:
            firstLineState();
            break;
        case State::HeaderState:
            headersState();
            break;
        case State::LoadBodyState:
            if(!loadBodyState()){
                return;
            }
            break;
        case State::BodyState:
            bodyState();
            break;
        case State::EndState:
            return endState();
        }

        if (!m_isValid) {
            m_state = State::EndState;
        }
    }
}

void IHttpRequestImpl::firstLineState()
{
    parseFirstLine(m_data.readLine());
    if (!m_isValid) {
        return;
    }

    resolveFirstLine();
    if(!m_isValid){
        return;
    }

    IHttpManage::instance().invokeFilters<IHttpFilterWare::Type::FirstLine>(m_request);
    if(!m_isValid){
        return;
    }

    if(m_data.getUnResolvedDataView().startWith(IHttp::NEW_LINE)){
        m_data.m_parsedSize += 2;
        m_requestComplete = true;
        m_state = State::EndState;
    }else{
        m_state = State::HeaderState;
    }
}

void IHttpRequestImpl::headersState()
{
    auto data = m_data.readPartition();

    parseHeaders(data);
    if (!m_isValid) {
        return;
    }

    resolveHeaders();
    if (!m_isValid) {
        return;
    }

    IHttpManage::instance().invokeFilters<IHttpFilterWare::Type::Header>(m_request);
    if(!m_isValid){
        return;
    }

    if(m_reqRaw.m_contentLength || m_reqRaw.m_isChunked){
        if(m_reqRaw.m_isChunked){
            m_chunkFlow = new IHttpChunkedFlow;
        }
        m_state = State::LoadBodyState;
        return;
    }else{
        m_requestComplete = true;
        m_state = State::EndState;
    }
}

bool IHttpRequestImpl::loadBodyState()
{
    if (m_reqRaw.m_isChunked) {
        return prepareChunkedData();
    }
    return prepareContentLengthData();
}

void IHttpRequestImpl::bodyState()
{
    resolvePayload();
    if(!m_isValid){
        return;
    }

    IHttpManage::instance().invokeFilters<IHttpFilterWare::Type::Body>(m_request);
    if(!m_isValid){
        return;
    }

    m_state = State::EndState;
}

void IHttpRequestImpl::endState()
{
    if (!m_requestComplete) {
        m_connection.m_keepAlive = false;
    }
    m_connection.doReadFinished();

    parseAction();
    IHttpManage::instance().invokeFilters<IHttpFilterWare::Type::PreHandle>(m_request);
    this->m_action->invoke(m_request);
}

bool IHttpRequestImpl::prepareContentLengthData()
{
    auto unparsedLength = m_data.getUnparsedLength();
    if(m_data.m_isConstBuffer && (unparsedLength >= m_reqRaw.m_contentLength)){
        m_reqRaw.m_body = m_data.readBy(m_reqRaw.m_contentLength);
        m_requestComplete = true;
        m_state = State::BodyState;
        return true;
    }

    if(!m_data.m_isConstBuffer && (m_data.m_buffer.size() >= m_reqRaw.m_contentLength)){
        m_reqRaw.m_body = m_data.getBufferView(m_reqRaw.m_contentLength);
        m_requestComplete = true;
        m_state = State::BodyState;
        return true;
    }

    if(m_data.m_isConstBuffer && (m_reqRaw.m_contentLength + m_data.m_parsedSize >= m_data.m_maxSize)){  // 数据超出
        m_data.m_isConstBuffer = false;
        auto data = asio::buffer_cast<char*>(m_data.m_buffer.prepare(m_reqRaw.m_contentLength));
        memcpy(data, m_data.m_data + m_data.m_parsedSize, unparsedLength); // 拷贝数据
        m_data.m_buffer.commit(unparsedLength);
    }
    m_connection.doReadBy(m_reqRaw.m_contentLength - unparsedLength);
    return false;
}

bool IHttpRequestImpl::prepareChunkedData()
{
    // shift data into buffer
    if(m_data.m_isConstBuffer){
        m_data.m_isConstBuffer = false;
        auto unparsedLength = m_data.getUnparsedLength();
        if(unparsedLength){
            auto len = unparsedLength * 3 > 512 ? unparsedLength*3 : 512;
            auto data = asio::buffer_cast<char*>(m_data.m_buffer.prepare(len));
            memcpy(data, m_data.m_data + m_data.m_parsedSize, unparsedLength); // 拷贝数据
            m_data.m_buffer.commit(unparsedLength);
        }
    }

    if(!tryParseChunkedData()){
        return false;
    }

    if(m_isValid){
        m_state = State::BodyState;
        m_requestComplete = true;
        m_reqRaw.m_body = montageChunkedData();
    }
    return true;
}

void IHttpRequestImpl::parseFirstLine(IStringView line)
{
    line = line.trimmed();

    // bad regex!!!
    //static const std::regex URI_REGEX(R"(^(\/[\w\-._~%!$&'()*+,;=:@/]*(\?[\w\-._~%!$&'()*+,;=:@/?]*)?|https?:\/\/[\w\-._~%!$&'()*+,;=:@]+\.[\w\-._~%!$&'()*+,;=:@]+(:\d+)?(\/[\w\-._~%!$&'()*+,;=:@/]*(\?[\w\-._~%!$&'()*+,;=:@/?]*)?)?|[*])$)");
    static $UInt urlMaxLength{ "/http/urlMaxLength", 1024 * 8 };
    if (line.length() >= *urlMaxLength) {
        return setInvalid(IHttpInvalidWare(IHttpStatus::URI_TOO_LONG_414));
    }

    std::size_t pos{};
    // method
    auto index = line.find(' ', pos);
    if (index == std::string::npos) {
        return setInvalid(IHttpBadRequestInvalid("can not resolve current method type"));
    }

    auto method = line.substr(pos, index);
    m_reqRaw.m_method = IHttpMethodUtil::toMethod(method);
    if (m_reqRaw.m_method == IHttpMethod::UNKNOWN) {
        return setInvalid(IHttpInvalidWare(IHttpStatus::METHOD_NOT_ALLOWED_405));
    }
    pos = index + 1;

    // path
    index = line.find(' ', pos);
    if (index == std::string_view::npos) {
        return setInvalid(IHttpBadRequestInvalid("request path is not correct"));
    }

    m_reqRaw.m_rawUrl = line.substr(pos, index - pos);
//    if (!std::regex_match(m_reqRaw.m_rawUrl.begin(), m_reqRaw.m_rawUrl.end(), URI_REGEX)) {
//        return setInvalid(IHttpBadRequestInvalid("url not valid"));
//    }
    pos = index + 1;

    // version
    m_reqRaw.m_httpVersion = IHttpVersionUtil::toVersion(line.substr(pos));
    if (m_reqRaw.m_httpVersion == IHttpVersion::UNKNOWN) {
        return setInvalid(IHttpInvalidWare(IHttpStatus::HTTP_VERSION_NOT_SUPPORTED_505));
    }
}

void IHttpRequestImpl::resolveFirstLine()
{
    auto index = m_reqRaw.m_rawUrl.find('?');
    if (index == std::string_view::npos) {
        m_reqRaw.m_rawPath = m_reqRaw.m_rawUrl;
        m_reqRaw.m_url = stash(QByteArray::fromPercentEncoding(QByteArray(m_reqRaw.m_rawUrl.data(), static_cast<int>(m_reqRaw.m_rawUrl.length()))));
        return;
    }

    m_reqRaw.m_rawPath = m_reqRaw.m_rawUrl.substr(0, index);
    m_reqRaw.m_url = stash(QByteArray::fromPercentEncoding(QByteArray(m_reqRaw.m_rawPath.data(), static_cast<int>(m_reqRaw.m_rawPath.length()))));

    m_reqRaw.m_rawPathArgs = m_reqRaw.m_rawUrl.substr(index + 1);
    parseUrlEncodedData(m_reqRaw.m_rawPathArgs, false);
}

void IHttpRequestImpl::parseHeaders(IStringView data)
{
    auto lines = data.trimmed().split(IHttp::NEW_LINE);
    for (auto line : lines) {
        parseHeader(line);
        if(!m_isValid){
            return;
        }
    }
}

void IHttpRequestImpl::parseHeader(IStringView line)
{
    static $UInt headerMaxLength("/http/headerMaxLength", 1024 * 8);
    if (line.length() > *headerMaxLength) {
        return setInvalid(IHttpInvalidWare(IHttpStatus::REQUEST_HEADER_FIELDS_TOO_LARGE_431));
    }

    auto index = line.find(':');
    if (index == std::string_view::npos) {
        return setInvalid(IHttpBadRequestInvalid("server do not support headers item multiline, or without key/value pair"));  // SEE: 默认不支持 headers 换行书写
    }

    auto key = line.substr(0, index).trimmed();
    auto value = line.substr(index + 1).trimmed();
    if (key.equalIgnoreCase(IHttpHeader::Cookie)) {
        return parseCookie(value);
    }

    for(auto it=m_reqRaw.m_headers.begin(); it !=m_reqRaw.m_headers.end(); it++){
        if(it.key().equalIgnoreCase(key)){
            return m_request.setInvalid(IHttpBadRequestInvalid("header duplicated"));
        }
    }

    m_reqRaw.m_headers[key] = value;
}

void IHttpRequestImpl::parseCookie(IStringView cookie)
{
    auto pairs = cookie.split(IHttp::SEMICOLON);
    for (IStringView pair : pairs) {
        auto val = pair.trimmed();
        if (!val.empty()) {
            auto args = val.split(IHttp::EQUAL);
            if (args.length() == 1) {
                m_reqRaw.m_cookies.insertMulti(stash(args.first()), stash(args.first()));
            }
            else {
                m_reqRaw.m_cookies.insertMulti(stash(args.first()), stash(args.last()));
            }
        }
    }
}

void IHttpRequestImpl::resolveHeaders()
{
    auto funs = std::vector{
        &IHttpRequestImpl::resolveHeaderContentDataType,
        &IHttpRequestImpl::resolveHeaderContentType,
        &IHttpRequestImpl::resolveHeaderKeepAlive
    };
    for(auto fun : funs){
        std::mem_fn(fun)(this);
        if(!m_isValid){
            return;
        }
    }
}

void IHttpRequestImpl::resolvePayload()
{
    switch (m_reqRaw.m_mime) {
    case IHttpMime::MULTIPART_FORM_DATA:
        parseMultiPartData(m_reqRaw.m_body);
        break;
    case IHttpMime::APPLICATION_WWW_FORM_URLENCODED:
        parseUrlEncodedData(m_reqRaw.m_body, true);
        break;
    case IHttpMime::APPLICATION_JSON:
    case IHttpMime::APPLICATION_JSON_UTF8:
        parseJsonData(m_reqRaw.m_body);
        break;
    default:
        break;
    }
}

void IHttpRequestImpl::parseUrlEncodedData(IStringView view, bool isBody)
{
    auto parts = view.split("&");
    for (auto part : parts) {
        if (part.empty()) {
            continue;
        }
        IStringViewList pair = part.split("=");
        if (pair.length() != 2) {
            setInvalid(IHttpBadRequestInvalid("form data error"));
            return;
        }
        auto first = pair.first().trimmed();
        auto last = pair.last().trimmed();
        auto key = stash(QByteArray::fromPercentEncoding(QByteArray(first.data(), static_cast<int>(first.length()))));
        auto value = stash(QByteArray::fromPercentEncoding(QByteArray(last.data(), static_cast<int>(last.length()))));
        if (isBody) {
            m_reqRaw.m_forms[key] = value;
        }
        else {
            m_reqRaw.m_queries[key] = value;
        }
    }
}

void IHttpRequestImpl::parseJsonData(IStringView data)
{
    m_reqRaw.m_json = IJson::parse(data.begin(), data.end(), nullptr, true);
    if (m_reqRaw.m_json.is_discarded()) {
        setInvalid(IHttpBadRequestInvalid("json error"));
    }
}

void IHttpRequestImpl::parseMultiPartData(IStringView data)
{
    data = data.trimmed();
    if (!data.startWith(m_multipartBoundary)) {
        return setInvalid(IHttpBadRequestInvalid("multipart start error"));
    }
    data = data.substr(m_multipartBoundary.length()).trimmed();

    auto endPos = data.find(m_multipartBoundaryEnd);
    if (endPos == IStringView::npos) {
        return setInvalid(IHttpBadRequestInvalid("multipart end error"));
    }

    data = data.substr(0, endPos);

    auto values = data.split(m_multipartBoundary);
    for (IStringView value : values) {
        if (!value.trimmed().empty()) {
            IHttpMultiPart part(value, &m_request);
            if (!part.isValid()) {
                return;
            }
            m_reqRaw.m_multiParts.emplace_back(std::move(part));
        }
    }
}

void IHttpRequestImpl::parseAction()
{
    m_action = IHttpManage::instance().getAction(m_request);
}

IStringView IHttpRequestImpl::getBoundary(IStringView data)
{
    static const std::string prefix = "boundary=";
    auto index = data.find(prefix);
    if (index == std::string_view::npos) {
        return {};
    }

    auto view = data.substr(index + prefix.length());
    if (view.empty()) {
        return {};
    }

    if (view.startWith("\"") && view.endWith("\"")) {
        view = view.substr(1, view.length() - 2);
    }
    return stash("--" + view.toStdString());
}

static IResult<std::size_t> toSizeValue(IStringView data)
{
    std::size_t value = 0;
    try {
        value = std::stoul(std::string(data), nullptr, 16);
    }
    catch (...) {
        return std::nullopt;
    }
    return value;
}

bool IHttpRequestImpl::tryParseChunkedData()
{
    auto data = m_data.getBufferView();
    if(data.empty()){
        m_connection.doReadStreamUntil(IHttp::NEW_PART, m_chunkFlow->m_chunkedPos);
        return false;
    }

    if(!m_chunkFlow->m_isChunkedBodyLoaded){
        while(true){
            auto end = data.find(IHttp::NEW_LINE, m_chunkFlow->m_chunkedPos);
            if(end == std::string_view::npos){
                m_connection.doReadStreamUntil(IHttp::NEW_PART, m_chunkFlow->m_chunkedPos);
                return false;
            }

            auto sizeVal = toSizeValue(data.substr(m_chunkFlow->m_chunkedPos, end-m_chunkFlow->m_chunkedPos));
            if(!sizeVal){
                setInvalid(IHttpBadRequestInvalid("Invalid chunk size:"));
                return false;
            }

            auto dataPos = end + 2;
            std::size_t size = *sizeVal;

            if(size == 0){
                if(dataPos + 2 > data.length()){
                    m_connection.doReadStreamUntil(IHttp::NEW_PART, dataPos - 2);
                    return false;
                }
                if(data.substr(dataPos, 2) == IHttp::NEW_LINE){
                    m_chunkFlow->m_chunkedPos = dataPos + 2;
                    m_chunkFlow->m_isChunkedBodyLoaded = true;
                    return true;
                }else{
                    m_chunkFlow->m_chunkedPos = dataPos;
                    m_chunkFlow->m_isChunkedBodyLoaded = true;
                    break;
                }
            }

            if(dataPos + (size + 2) > data.length()){
                m_connection.doReadStreamUntil(IHttp::NEW_PART, dataPos + 2);
                return false;
            }

            m_chunkFlow->m_chunkedDataStash.push_back(std::pair<int, int>{dataPos, size});

            if (data.substr(dataPos + size, 2) != IHttp::NEW_LINE) {       // check \r\n after data
                setInvalid(IHttpBadRequestInvalid("Invalid chunked data: missing chunk data terminator"));
                return false;
            }

            m_chunkFlow->m_chunkedPos = dataPos + size + 2;
        }
    }

    if(m_chunkFlow->m_isChunkedBodyLoaded){  // parse trailer
        auto pos = data.find(IHttp::NEW_PART, m_chunkFlow->m_chunkedPos);
        if(pos == std::string_view::npos){
            m_connection.doReadStreamUntil(IHttp::NEW_PART, m_chunkFlow->m_chunkedPos);
            return false;
        }

        auto view = data.substr(m_chunkFlow->m_chunkedPos).trimmed();
        auto lines = view.split(IHttp::NEW_LINE);
        for(auto line : lines){
            auto colonPos = line.find(":");
            if(colonPos == std::string_view::npos){
                setInvalid(IHttpBadRequestInvalid("Invalid chunked data: trailer data not legal"));
                return false;
            }

            auto key = line.substr(0, colonPos).trimmed();
            auto value = line.substr(colonPos + 1).trimmed();
            m_reqRaw.m_headers[key] = value;
        }
        m_chunkFlow->m_chunkedPos = pos + 4;
        m_requestComplete = true;
    }

    return true;
}

IStringView IHttpRequestImpl::montageChunkedData()
{
    std::size_t len = 0;
    for(const auto& pair : m_chunkFlow->m_chunkedDataStash){
        len += pair.second;
    }
    std::string data;
    data.reserve(len);
    auto ptr2 = m_data.getBufferView().data();
//    IStringView view = m_data.getBufferView();
    for(const auto& pair : m_chunkFlow->m_chunkedDataStash){
        data += std::string(ptr2 + pair.first, pair.second);
//        memcpy(ptr, ptr2 + pair.first, pair.second);
//        ptr += pair.second;
    }
    return stash(data);
}

IStringView IHttpRequestImpl::parseChunkedData(IStringView data) {
    std::string result;
    result.reserve(data.length());
    std::size_t pos{};
    while (true) {
        auto end = data.find(IHttp::NEW_LINE, pos);
        if (end == std::string_view::npos) {
            setInvalid(IHttpBadRequestInvalid("Invalid chunked data: missing chunk size delimiter"));
            return {};
        }
        std::string_view chunkedSize = data.substr(pos, end - pos);
        size_t chunk_size = 0;
        try {
            chunk_size = std::stoul(std::string(chunkedSize), nullptr, 16);
        }
        catch (...) {
            setInvalid(IHttpBadRequestInvalid("Invalid chunk size:"));
            return {};
        }
        pos = end + 2;
        if (chunk_size == 0) {
            pos += 2;
            break;
        }
        if (pos + chunk_size > data.size()) {
            setInvalid(IHttpBadRequestInvalid("Invalid chunked data: missing chunk terminator"));
            return {};
        }

        result.append(data.substr(pos, chunk_size));
        pos += chunk_size;
        if (!data.substr(pos, 2).equalIgnoreCase(IHttp::NEW_LINE)) {
            setInvalid(IHttpBadRequestInvalid("Invalid chunked data: missing chunk terminator"));
            return {};
        }
        pos += 2;
    }
    while (pos < data.size()) {
        auto end = data.find(IHttp::NEW_LINE, pos);
        if (end == std::string_view::npos) {
            setInvalid(IHttpBadRequestInvalid("Invalid chunked data: incomplete trailer"));
            return {};
        }
        if (end == pos) {
            pos += 2;
            break;
        }
        auto line = data.substr(pos, end - pos);
        auto colonPos = line.find(":");
        auto key = line.substr(0, colonPos).trimmed();
        auto value = line.substr(colonPos + 1).trimmed();
        m_reqRaw.m_headers[key] = value;
        pos = end + 2;
    }
    m_reqRaw.m_contentLength = result.length();
    return stash(std::move(result));
}

void IHttpRequestImpl::resolveHeaderContentDataType()
{
    auto encoding = m_headerJar.getRequestHeaderValue(IHttpHeader::TransferEncoding);
    if (encoding.length()) {
        if (encoding.equalIgnoreCase("chunked")) {
            m_reqRaw.m_isChunked = true;
        }
    }

    auto contentLength = m_headerJar.getRequestHeaderValue(IHttpHeader::ContentLength);
    if (contentLength.length()) {
        if (m_reqRaw.m_isChunked) {
            return setInvalid(IHttpBadRequestInvalid("ContentLength and chunked exist at same time"));
        }
        auto len = IString(contentLength).value<int>();
        if (!len) {
            return setInvalid(IHttpBadRequestInvalid("ContentLength error"));
        }else{
            m_reqRaw.m_contentLength = *len;
        }
        static $UInt bodyMaxLength("/http/bodyMaxLength", 4194304);
        if (m_reqRaw.m_contentLength > *bodyMaxLength) {
            return setInvalid(IHttpBadRequestInvalid("Content Length too large to accept"));
        }
    }
}

void IHttpRequestImpl::resolveHeaderContentType()
{
    auto contentType = m_headerJar.getRequestHeaderValue(IHttpHeader::ContentType);
    if (contentType.length()) {
        m_reqRaw.m_mime = IHttpMimeUtil::toMime(contentType);
        if (m_reqRaw.m_mime == IHttpMime::MULTIPART_FORM_DATA) {
            m_multipartBoundary = getBoundary(contentType);
            if (m_multipartBoundary.empty()) {
                return setInvalid(IHttpBadRequestInvalid("multipart request has no boundary"));
            }
            m_multipartBoundaryEnd = stash(m_multipartBoundary.toQByteArray() + "--");
        }
    }
}

void IHttpRequestImpl::resolveHeaderKeepAlive()
{
    if (m_reqRaw.m_httpVersion == IHttpVersion::VERSION_1_0) {
        m_connection.m_keepAlive = false;
    } else {
        m_connection.m_keepAlive = true;
    }
    auto connection = m_headerJar.getRequestHeaderValue(IHttpHeader::Connection);
    if (connection.length() != 0) {
        if (connection.containIgnoreCase("Keep-Alive")) {
            m_connection.m_keepAlive = true;
        }else if (connection.containIgnoreCase("close")) {
            m_connection.m_keepAlive = false;
        }
    }
}

void IHttpRequestImpl::setInvalid(const IHttpInvalidWare& ware)
{
    m_isValid = false;
    m_respRaw.setContent(ware);
}

void IHttpRequestImpl::setResponseWare(const IHttpResponseWare &ware)
{
    m_isValid &= ware.m_raw->m_isValid;
    m_respRaw.setContent(ware);
}

bool IHttpRequestImpl::isSessionExist() const
{
    return m_session != nullptr;
}

IHttpSession &IHttpRequestImpl::session()
{
    static $Bool enabled{"/http/session/enabled", false};
    if(! *enabled){
        qFatal("Session not enabled, please check your configuration");
    }

    if(!m_session){
        m_session = new IHttpSession(*this);
    }
    return *m_session;
}

$PackageWebCoreEnd
