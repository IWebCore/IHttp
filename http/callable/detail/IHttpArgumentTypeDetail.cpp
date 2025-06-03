#include "IHttpArgumentTypeDetail.h"
#include "core/util/ISpawnUtil.h"
#include "core/bean/IBeanTypeManage.h"
#include "http/controller/IHttpControllerAbort.h"
#include "http/controller/IHttpControllerAction.h"
#include "http/IHttpAbort.h"
#include "http/IHttpCookieJar.h"
#include "http/IHttpManage.h"
#include "http/IRequest.h"
#include "http/IResponse.h"
#include "http/IHttpSession.h"
#include "http/IHttpMultiPartJar.h"
#include "http/IHttpHeaderJar.h"
#include "http/detail/IHttpRequestImpl.h"
#include "http/invalid/IHttpInternalErrorInvalid.h"
#include "http/invalid/IHttpBadRequestInvalid.h"

$PackageWebCoreBegin

namespace detail
{
    static bool isMultipleType(QMetaType::Type typeId, const IString& typeName)
    {
        if(typeId == QMetaType::QStringList){
            return true;
        }
        if(typeName == "QList<IString>"){
            return true;
        }
        return false;
    }

    static bool isTypeConvertable(QMetaType::Type typeId, const IString& typeName)
    {
        static QList<QMetaType::Type> types = {
            QMetaType::Bool, QMetaType::SChar, QMetaType::UChar,
            QMetaType::UShort, QMetaType::Short, QMetaType::Int, QMetaType::UInt,
            QMetaType::ULong, QMetaType::Long, QMetaType::ULongLong, QMetaType::LongLong,
            QMetaType::Float, QMetaType::Double, QMetaType::QString, QMetaType::QByteArray,
            QMetaType::QDate, QMetaType::QDateTime, QMetaType::QTime,
        };
        static QList<IString> typeNames = {
            "IString", "std::string", "IStringView", "QList<IString>", "QStringList",
            "IHttpMultiPart", "IHttpCookiePart"
        };
        return types.contains(typeId) || typeNames.contains(typeName);
    }

//    static void* convertPtr(const QList<IString>& data, QMetaType::Type typeId, const IString& typeName)
//    {
//        if(typeId == QMetaType::QStringList){
//            auto ret = new QStringList;
//            for(const auto& val : data){
//                ret->append(val.toQString());
//            }
//            return ret;
//        }
//        if(typeName == "QList<IString>") {
//            return new QList<IString>(data);
//        }
//        IHttpAbort::abortTypeConvertionFailed("can not convert from QList<IString> to " + typeName.toQString(), $ISourceLocation);
//        return nullptr;
//    }

    static void* convertPtr(const IStringViewList& data, QMetaType::Type typeId, const IString& typeName)
    {
        if(typeId == QMetaType::QStringList){
            auto ret = new QStringList;
            for(const auto& val : data){
                ret->append(val.toQString());
            }
            return ret;
        }
        if(typeName == "QList<IString>") {
            auto ret = new QList<IString> ;
            for(auto value : data){
                ret->append(value);
            }
            return ret;
        }
        IHttpAbort::abortTypeConvertionFailed("can not convert from IStringViewList to " + typeName.toQString(), $ISourceLocation);
        return nullptr;
    }

    template<typename T>
    T* valueToPtr(const IString& data)
    {
        auto value = data.value<T>();
        if(value){
            return new T(*value);
        }
        return nullptr;
    }

    static void* convertPtr(const IString& data, QMetaType::Type typeId, const IString& typeName)
    {
        switch (typeId) {
        case QMetaType::Bool:
            return valueToPtr<bool>(data);
        case QMetaType::SChar:
            return valueToPtr<signed char>(data);
        case QMetaType::UChar:
            return valueToPtr<unsigned char>(data);
        case QMetaType::Short:
            return valueToPtr<signed short>(data);
        case QMetaType::UShort:
            return valueToPtr<unsigned short>(data);
        case QMetaType::Int:
            return valueToPtr<signed int>(data);
        case QMetaType::UInt:
            return valueToPtr<unsigned int>(data);
        case QMetaType::Long:
            return valueToPtr<signed long>(data);
        case QMetaType::ULong:
            return valueToPtr<unsigned long>(data);
        case QMetaType::LongLong:
            return valueToPtr<signed long long>(data);
        case QMetaType::ULongLong:
            return valueToPtr<unsigned long long>(data);
        case QMetaType::Float:
            return valueToPtr<float>(data);
        case QMetaType::Double:
            return valueToPtr<double>(data);
        case QMetaType::QString:
            return valueToPtr<QString>(data);
        case QMetaType::QByteArray:
            return valueToPtr<QByteArray>(data);
        case QMetaType::QDate:
            return valueToPtr<QDate>(data);
        case QMetaType::QTime:
            return valueToPtr<QTime>(data);
        case QMetaType::QDateTime:
            return valueToPtr<QDateTime>(data);
        default:
            break;
        }
        if(typeName == "IString"){
            return valueToPtr<IString>(data);
        }else if(typeName == "std::string"){
            return valueToPtr<std::string>(data);
        }else if(typeName == "IStringView"){
            return valueToPtr<IStringView>(data);
        }else if(typeName == "IHttpMultiPart"){
            if(!data.isEmpty()){
                IHttpAbort::abortArgumentTypeError("in multiPart, no optional variable needed", $ISourceLocation);
            }
            return const_cast<IHttpMultiPart*>(&IHttpMultiPart::Empty);
        }else if(typeName == "IHttpCookiePart"){
            if(!data.isEmpty()){
                IHttpAbort::abortArgumentTypeError("in cookiepart, no optional variable needed", $ISourceLocation);
            }
            return const_cast<IHttpCookiePart*>(&IHttpCookiePart::Empty);
        }
        IHttpAbort::abortTypeConvertionFailed("can not convert from IString to " + typeName.toQString(), $ISourceLocation);
        return nullptr;
    }

    static void deletePtr(void* ptr, QMetaType::Type typeId, const IString& typeName)
    {
        if(!ptr){
            return;
        }
        switch (typeId) {
        case QMetaType::Bool:
            return delete static_cast<bool*>(ptr);
        case QMetaType::SChar:
            return delete static_cast<signed char*>(ptr);
        case QMetaType::UChar:
            return delete static_cast<unsigned char*>(ptr);
        case QMetaType::Short:
            return delete static_cast<short*>(ptr);
        case QMetaType::UShort:
            return delete static_cast<unsigned short*>(ptr);
        case QMetaType::Int:
            return delete static_cast<int*>(ptr);
        case QMetaType::UInt:
            return delete static_cast<unsigned int*>(ptr);
        case QMetaType::Long:
            return delete static_cast<long*>(ptr);
        case QMetaType::ULong:
            return delete static_cast<unsigned long*>(ptr);
        case QMetaType::LongLong:
            return delete static_cast<long long*>(ptr);
        case QMetaType::ULongLong:
            return delete static_cast<unsigned long long*>(ptr);
        case QMetaType::Float:
            return delete static_cast<float*>(ptr);
        case QMetaType::Double:
            return delete static_cast<double*>(ptr);
        case QMetaType::QString:
            return delete static_cast<QString*>(ptr);
        case QMetaType::QByteArray:
            return delete static_cast<QByteArray*>(ptr);
        case QMetaType::QStringList:
            return delete static_cast<QStringList*>(ptr);
        case QMetaType::QDate:
            return delete static_cast<QDate*>(ptr);
        case QMetaType::QTime:
            return delete static_cast<QTime*>(ptr);
        case QMetaType::QDateTime:
            return delete static_cast<QDateTime*>(ptr);
        default:
            break;
        }
        if(typeName == "IString"){
            return delete static_cast<IString*>(ptr);
        }else if(typeName == "std::string"){
            return delete static_cast<std::string*>(ptr);
        }else if(typeName == "IStringView"){
            return delete static_cast<IStringView*>(ptr);
        }else if(typeName == "QList<IString>"){
            return delete static_cast<QList<IString>*>(ptr);
        }
        IHttpAbort::abortArgumentTypeError("error when delete refactored value. type:" + typeName.toQString());
    }
}

IHttpArgumentTypeDetail::IHttpArgumentTypeDetail(int typeId, QByteArray paramTypeName, QByteArray nameRaw)
    : IHttpArgumentType()
{
    m_typeId = QMetaType::Type(typeId);
    m_typeName = std::move(paramTypeName);
    m_nameRaw = std::move(nameRaw);
    resolveName();

    static QList<decltype(&IHttpArgumentTypeDetail::createBasicTypes)> funs = {
        &IHttpArgumentTypeDetail::createBasicTypes,
        &IHttpArgumentTypeDetail::createPartTypes,
        &IHttpArgumentTypeDetail::createDecorateTypes,
        &IHttpArgumentTypeDetail::createBeanTypes,
    };
    for(auto fun : funs){
        std::invoke(fun, this);
        if(m_createFun){
            return;
        };
    }
    IHttpAbort::abortArgumentTypeError("parse argument type failed, this type and name can not be managed", $ISourceLocation);
}

void IHttpArgumentTypeDetail::resolveName()
{
    static const QVector<IString> PREFIXES = {
        "auto", "PATH", "QUERY", "HEADER", "COOKIE", "SESSION", "BODY", "FORM", "JSON"
    };
    if(m_nameRaw.isEmpty()){
        IHttpAbort::abortArgumentTypeError("Http Method declaration should not only contain type, but also name. name can not be empty!");
    }
    qDebug() << m_nameRaw.toQString();
    IStringViewList args = m_nameRaw.split("_$");
    m_name = args.first();
    args.pop_front();

    for(IStringView arg : args){
        auto index = PREFIXES.indexOf(arg);
        if(index > 0){
            if(m_position > 0){
                IHttpAbort::abortArgumentTypeError("two more restrict. currently we only support one POSITION restriction");
            }
            m_position = Position(index);
        }

        // optional
        if(arg.startWith("OPTIONAL")){
            if(m_optional == true){
                IHttpAbort::abortArgumentTypeError("two more restrict. currently we only support one POSITION restriction", $ISourceLocation);
            }
            m_optional = true;
            auto spliter = arg.find("$$");
            if(spliter != std::string_view::npos){
                m_optionalString = arg.substr(spliter+2);
            }
            if(detail::isTypeConvertable(m_typeId, m_typeName)){
                m_optionalPtr = detail::convertPtr(m_optionalString, m_typeId, m_typeName);
                if(!m_optionalPtr){
                    IHttpAbort::abortArgumentTypeError("optional can not be converted", $ISourceLocation);
                }
            }else{
                IHttpAbort::abortArgumentTypeError("optional not convertable", $ISourceLocation);
            }
        }
    }
}

void IHttpArgumentTypeDetail::createBasicTypes()
{
    static QList<decltype(&IHttpArgumentTypeDetail::createRequestType)> funs = {
        &IHttpArgumentTypeDetail::createRequestType,
        &IHttpArgumentTypeDetail::createResponseType,
        &IHttpArgumentTypeDetail::createMultiPartJarType,
        &IHttpArgumentTypeDetail::createCookieJarType,
        &IHttpArgumentTypeDetail::createSessionJarType,
        &IHttpArgumentTypeDetail::createHeaderJarType,
    };

    if(this->m_typeId != QMetaType::UnknownType){
        return;
    }

    for(auto fun : funs){
        std::invoke(fun, this);
        if(m_createFun){
            if(m_optional){
                IHttpAbort::abortArgumentTypeError("optional can not be exist here", $ISourceLocation);
            }
            if(m_position != Position::Auto){
                IHttpAbort::abortArgumentTypeError("position should be empty", $ISourceLocation);
            }
            return;
        }
    }
}

void IHttpArgumentTypeDetail::createRequestType()
{
    static const auto types = makeTypes("IRequest");
    if(types.contains(this->m_typeName)){
        this->m_createFun = [](IRequest& request)->void*{
            return &request;
        };
    }
}

void IHttpArgumentTypeDetail::createResponseType()
{
    static const auto types = makeTypes("IResponse");
    if(types.contains(this->m_typeName)){
        this->m_createFun = [](IRequest& request)->void*{
            return new IResponse(request);
        };
        this->m_destroyFun = [](void* ptr){
            return delete static_cast<IResponse*>(ptr);
        };
    }
}

void IHttpArgumentTypeDetail::createMultiPartJarType()
{
    static const auto types = makeTypes("IHttpMultiPartJar");
    if(types.contains(this->m_typeName)){
        this->m_createFun = [](IRequest& request)->void*{
            return &(request.impl().m_multiPartJar);
        };
    }
}

void IHttpArgumentTypeDetail::createSessionJarType()
{
    static const auto types = makeTypes("IHttpSession");
    if(types.contains(this->m_typeName)){
        this->m_createFun = [](IRequest& request)->void*{
            return &(request.impl().session());
        };
    }
}

void IHttpArgumentTypeDetail::createCookieJarType()
{
    static const auto types = makeTypes("IHttpCookieJar");
    if(types.contains(this->m_typeName)){
        this->m_createFun = [](IRequest& request)->void*{
            return &(request.impl().m_cookieJar);
        };
    }
}

void IHttpArgumentTypeDetail::createHeaderJarType()
{
    static const auto types = makeTypes("IHttpHeaderJar");
    if(types.contains(this->m_typeName)){
        this->m_createFun = [](IRequest& request)->void*{
            return &(request.impl().m_headerJar);
        };
    }
}

void IHttpArgumentTypeDetail::createPartTypes()
{
    if(this->m_typeId != QMetaType::UnknownType){
        return;
    }
    static QList<decltype(&IHttpArgumentTypeDetail::createRequestType)> funs = {
        &IHttpArgumentTypeDetail::createMultiPartType,
        &IHttpArgumentTypeDetail::createCookiePartType,
    };
    for(auto fun : funs){
        std::invoke(fun, this);
        if(m_createFun){
            return;
        }
    }
}

void IHttpArgumentTypeDetail::createMultiPartType()
{
    static const auto types = makeTypes("IHttpMultiPart");
    if(!types.contains(this->m_typeName)){
        return;
    }
    if(m_position != Position::Auto){
        IHttpAbort::abortArgumentTypeError("Position should not exist when data type is IHttpMultiPart", $ISourceLocation);
    }
    auto self = *this;
    this->m_createFun = [=](IRequest& request) -> void*{
        if(request.mime() == IHttpMime::MULTIPART_FORM_DATA){
            const auto& value = request.multiPartJar().getMultiPart(self.m_name);
            if(&value == &IHttpMultiPart::Empty){
                if(self.m_optional){
                    return self.m_optionalPtr;
                }
                request.setInvalid(IHttpInternalErrorInvalid("multitype not optional"));
                return nullptr;
            }
            return static_cast<void*>(const_cast<IHttpMultiPart*>(&value));
        }
        request.setInvalid(IHttpInternalErrorInvalid("not multitype type"));
        return nullptr;
    };
}

void IHttpArgumentTypeDetail::createCookiePartType()
{
    static const auto& types = makeTypes("IHttpCookiePart");
    if(!types.contains(this->m_typeName)){
        return;
    }
    if(m_position != Position::Auto){
        IHttpAbort::abortArgumentTypeError("Position should not exist when data type is IHttpCookiePart", $ISourceLocation);
    }
    auto self = *this;
    this->m_createFun = [=](IRequest& request) -> void*{
        if(request.impl().m_reqRaw.m_cookies.contains(self.m_name)){
            const auto& value = request.impl().m_reqRaw.m_cookies.value(self.m_name);
            return new IHttpCookiePart(self.m_name, value);
        }
        if(self.m_optional){
            return m_optionalPtr;
        }
        request.setInvalid(IHttpInternalErrorInvalid("cookie not optional"));
        return nullptr;
    };
    this->m_destroyFun = [](void* ptr){
        if(ptr  && (ptr != &IHttpCookiePart::Empty)){
            delete static_cast<IHttpCookiePart*>(ptr);
        }
    };
}

void IHttpArgumentTypeDetail::createDecorateTypes()
{
    QList<decltype(&IHttpArgumentTypeDetail::createQueryType)> funs = {
        &IHttpArgumentTypeDetail::createQueryType,
        &IHttpArgumentTypeDetail::createHeaderType,
        &IHttpArgumentTypeDetail::createCookieType,
        &IHttpArgumentTypeDetail::createSessionType,
        &IHttpArgumentTypeDetail::createPathType,
        &IHttpArgumentTypeDetail::createBodyType,
        &IHttpArgumentTypeDetail::createFormType,
        &IHttpArgumentTypeDetail::createJsonType
    };

    for(auto fun : funs){
        std::invoke(fun, this);
        if(m_createFun){
            auto typeId = m_typeId;
            auto typeName = m_typeName;
            if(!this->m_destroyFun){
                this->m_destroyFun = [=](void* ptr){
                    detail::deletePtr(ptr, typeId, typeName);
                };
            }
            return;
        }
    }
}

void IHttpArgumentTypeDetail::createQueryType()
{
    if(this->m_position != Position::Query){
        return;
    }
    if(!detail::isTypeConvertable(m_typeId, m_typeName)){
        IHttpAbort::abortArgumentTypeError("type can not be processed. type:" + m_typeName.toQString(), $ISourceLocation);
    }
    auto self = *this;
    this->m_createFun = [=](IRequest& req)->void*{
        if(req.impl().m_reqRaw.m_queries.contains(self.m_name.m_view)){
            auto data = req.impl().m_reqRaw.m_queries[self.m_name];
            return detail::convertPtr(data, self.m_typeId, self.m_typeName);
        }
        if(self.m_optional){
            return self.m_optionalPtr;
        }
        req.setInvalid(IHttpInternalErrorInvalid("not found arg"));
        return nullptr;
    };
}

void IHttpArgumentTypeDetail::createHeaderType()
{
    if(this->m_position != Position::Header){
        return;
    }
    if(!detail::isTypeConvertable(m_typeId, m_typeName)){
        IHttpAbort::abortArgumentTypeError("type can not be processed. type:" + m_typeName.toQString(), $ISourceLocation);
    }
    auto self = *this;
    this->m_createFun = [=](IRequest& request) ->void*{

        const auto& keys = request.impl().m_reqRaw.m_headers.keys();
        for(const auto& key : keys){
            if(key.equalIgnoreCase(self.m_name)){
                auto value = request.impl().m_reqRaw.m_headers.value(key);
                auto ptr = detail::convertPtr(value, self.m_typeId, self.m_typeName);
                if(!ptr){
                    request.setInvalid(IHttpBadRequestInvalid("header field value not proper"));
                }
                return ptr;
            }
        }

        if(self.m_optional){
            return self.m_optionalPtr;
        }
        request.setInvalid(IHttpInternalErrorInvalid("header field not resolved"));
        return nullptr;
    };
}

void IHttpArgumentTypeDetail::createCookieType()
{
    if(this->m_position != Position::Cookie){
        return;
    }
    if(!detail::isTypeConvertable(m_typeId, m_typeName)){
        IHttpAbort::abortArgumentTypeError("type can not be processed. type:" + m_typeName.toQString(), $ISourceLocation);
    }
    auto self = *this;
    this->m_createFun = [=](IRequest& request) ->void*{
        if(detail::isMultipleType(self.m_typeId, self.m_typeName)){
            auto values = request.impl().m_reqRaw.m_cookies.values(self.m_name);
            if(!values.isEmpty()){
                auto ptr = detail::convertPtr(values, self.m_typeId, self.m_typeName);
                if(!ptr){
                    request.setInvalid(IHttpBadRequestInvalid("header field value not proper"));
                }
                return ptr;
            }
            if(self.m_optional){
                return self.m_optionalPtr;
            }
            request.setInvalid(IHttpInternalErrorInvalid("cookie missed"));
            return nullptr;
        }else{
            if(request.impl().m_reqRaw.m_cookies.contains(self.m_name)){
                auto value = request.impl().m_reqRaw.m_cookies.value(self.m_name);
                auto ptr = detail::convertPtr(value, self.m_typeId, self.m_typeName);
                if(!ptr){
                    request.setInvalid(IHttpBadRequestInvalid("header field value not proper"));
                }
                return ptr;
            }
            if(self.m_optional){
                return self.m_optionalPtr;
            }
            request.setInvalid(IHttpInternalErrorInvalid("cookie not found"));
            return nullptr;
        }
        return nullptr;
    };
}

void IHttpArgumentTypeDetail::createSessionType()
{

}

void IHttpArgumentTypeDetail::createPathType()
{
    if(this->m_position != Position::Path){
        return;
    }
    if(!detail::isTypeConvertable(m_typeId, m_typeName)){
        IHttpAbort::abortArgumentTypeError("type can not be processed. type:" + m_typeName.toQString(), $ISourceLocation);
    }
    if(this->m_optional){
        IHttpAbort::abortArgumentTypeError("Path variable can`t be optional", $ISourceLocation);
    }
    auto self = *this;
    this->m_createFun = [=](IRequest& req) -> void*{
        auto action = dynamic_cast<IHttpControllerAction*>(req.impl().m_action);
        auto index = action->m_path.m_actionNameMap[self.m_name];
        auto url = req.impl().m_reqRaw.m_url.split("/");
        auto value = url[index+1];      // 这里第一个数值是空
        auto ptr = detail::convertPtr(value, self.m_typeId, self.m_typeName);
        if(!ptr){
            req.setInvalid(IHttpBadRequestInvalid("path value can not be converted"));
        }
        return ptr;
    };
}

void IHttpArgumentTypeDetail::createBodyType()
{
    if(this->m_position != Position::Body){
        return;
    }
    if(!detail::isTypeConvertable(m_typeId, m_typeName)){
        IHttpAbort::abortArgumentTypeError("type can not be processed. type:" + m_typeName.toQString(), $ISourceLocation);
    }
    static const std::vector<IString> types = {"IString", "IStringView", "std::string", "QString", "QByteArray"};
    if(std::find(types.begin(), types.end(), m_typeName) == types.end()){
        IHttpAbort::abortArgumentTypeError("type must be string type, check it", $ISourceLocation); // TODO: 这个限制之后可以考虑放开。
    }
    auto self = *this;
    this->m_createFun = [=](IRequest& req)->void*{
        if(req.impl().m_reqRaw.m_contentLength == 0 && req.impl().m_reqRaw.m_isChunked==false){
            if(m_optional){
                return m_optionalPtr;
            }
            req.setInvalid(IHttpInternalErrorInvalid("request do not contain body"));
            return nullptr;
        }
        auto ptr = detail::convertPtr(req.impl().m_reqRaw.m_body, self.m_typeId, self.m_typeName);
        if(!ptr){
            req.setInvalid(IHttpBadRequestInvalid("body field can not be conveted"));
        }
        return ptr;
    };
}

void IHttpArgumentTypeDetail::createFormType()
{
    if(this->m_position != Position::Form){
        return;
    }
    if(!detail::isTypeConvertable(m_typeId, m_typeName)){
        IHttpAbort::abortArgumentTypeError("type can not be processed. type:" + m_typeName.toQString(), $ISourceLocation);
    }
    auto self = *this;
    this->m_createFun = [=](IRequest& req)->void*{
        if(req.mime() != IHttpMime::APPLICATION_WWW_FORM_URLENCODED){
            req.setInvalid(IHttpInternalErrorInvalid("request should be form data"));
            return nullptr;
        }
        if(req.impl().m_reqRaw.m_forms.contains(self.m_name.m_view)){
            auto data = req.impl().m_reqRaw.m_forms[self.m_name];
            auto ptr = detail::convertPtr(data, self.m_typeId, self.m_typeName);
            if(!ptr){
                req.setInvalid(IHttpBadRequestInvalid("form field value not proper"));
            }
            return ptr;
        }
        if(self.m_optional){
            return m_optionalPtr;
        }
        req.setInvalid(IHttpInternalErrorInvalid("not found form value"));
        return nullptr;
    };
}

void IHttpArgumentTypeDetail::createJsonType()
{
    static const auto types = makeTypes("IJson");
    if(!types.contains(m_typeName)){
        return;
    }
    if(m_position != Position::Auto && m_position != Position::Json){
        IHttpAbort::abortArgumentTypeError("in IJson type, postion must not exist or be $Json");
    }
    if(m_optional){
        IHttpAbort::abortArgumentTypeError("IJson can not be optional currently", $ISourceLocation);
    }

    auto self = *this;
    auto pointer = createJsonPointer(m_name);
    this->m_createFun = [=](IRequest& request)->void*{
        if(request.mime() != IHttpMime::APPLICATION_JSON
                && request.mime() != IHttpMime::APPLICATION_JSON_UTF8){
            request.setInvalid(IHttpInternalErrorInvalid("request do not contain json payload"));
            return nullptr;
        }
        const auto& json = request.impl().m_reqRaw.m_json;
        if(self.m_position == Position::Auto){
            return const_cast<IJson*>(&json);
        }
        if(json.contains(pointer)){
            return new IJson(json[pointer]);
        }
        request.setInvalid(IHttpInternalErrorInvalid("not found json body"));
        return nullptr;
    };
    this->m_destroyFun = [=](void* ptr){
        if(self.m_position == Position::Json){
            delete static_cast<IJson*>(ptr);
        }
    };
}

void IHttpArgumentTypeDetail::createBeanTypes()
{
    if(!IBeanTypeManage::instance().isBeanIdExist(m_typeId)){
        return;
    }
    auto self = *this;
    this->m_createFun = [=](IRequest& req)->void*{
        if(req.mime() != IHttpMime::APPLICATION_JSON && req.mime() != IHttpMime::APPLICATION_JSON_UTF8){
            req.setInvalid(IHttpBadRequestInvalid("bean only for json payload currently"));
            return nullptr;
        }
        auto ptr = QMetaType::create(self.m_typeId);
        auto fun = IBeanTypeManage::instance().getBeanFromJsonFun(self.m_typeId);
        if(!fun ||  !fun(ptr, req.bodyJson())){
            req.setInvalid(IHttpInternalErrorInvalid("json can not be converted to Bean"));
        }
        return ptr;
    };
    this->m_destroyFun = [=](void* ptr){
        if(ptr){
            QMetaType::destroy(self.m_typeId, ptr);
        }
    };
}

QVector<IString> IHttpArgumentTypeDetail::makeTypes(const std::string &name)
{
    return QVector<IString> {
        name, name + "&",
        std::string($PackageWebCoreName) + "::" + name,
        std::string($PackageWebCoreName) + "::" + name + "&"
    };
}

IJson::json_pointer IHttpArgumentTypeDetail::createJsonPointer(const IString & data)
{
    QString path = data.toQString().replace("$", "/");
    if(!path.startsWith("/")){
        path = "/"+path;
    }
    IJson::json_pointer pointer(path.toStdString());
    return pointer;
}

namespace ISpawnUtil
{
    template<>
    IHttpArgumentType construct(int paramTypeId, QByteArray paramTypeName, QByteArray paramName)
    {
        return IHttpArgumentTypeDetail(paramTypeId, std::move(paramTypeName), std::move(paramName));
    }
}

$PackageWebCoreEnd
