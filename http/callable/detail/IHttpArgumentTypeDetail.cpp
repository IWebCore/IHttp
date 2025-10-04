#include "IHttpArgumentTypeDetail.h"
#include "core/util/ISpawnUtil.h"
#include "core/util/IJsonUtil.h"
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
    static bool isStringListType(QMetaType::Type typeId, const IString& typeName)
    {
        return typeId == QMetaType::QStringList || typeName == "QList<IString>";
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
            "IString", "std::string", "IStringView",
            "QList<IString>", "QStringList",
        };
        return types.contains(typeId) || typeNames.contains(typeName);
    }

    template<typename T>
    T* createValue(const T& data, bool ok=true)
    {
        if(ok){
            return new T(data);
        }
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

    template<typename T>
    static void* convertFromIJsonToNumberPtr(const IJson& val){
        T t;
        if(IJsonUtil::fromJson(t, val)){
            return createValue(t);
        }
        return nullptr;
    }

    template<typename T>
    static void* convertFromIJsonToDatePtr(const IJson& val){
        if(!val.is_string()){
            return nullptr;
        }

        bool ok;
        T date;
        auto value = QString::fromStdString(val.get<std::string>());
        if constexpr (std::is_same_v<T, QDate>){
            date = IConvertUtil::toDate(value, ok);
        } else if constexpr(std::is_same_v<T, QTime>){
            date = IConvertUtil::toTime(value, ok);
        } else if constexpr(std::is_same_v<T, QDateTime>){
            date = IConvertUtil::toDateTime(value, ok);
        }else{
            qFatal("invalid invoke");
        }
        if(ok){
            return createValue(date);
        }
        return nullptr;
    }

    static void* fromIStringViewListToPtr(const IStringViewList& data, QMetaType::Type typeId, const IString& typeName)
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

    static void* fromDefaultValueToPtr(QMetaType::Type typeId, const IString& typeName)
    {
        switch (typeId) {
        case QMetaType::Bool:
            return createValue(false);
        case QMetaType::SChar:
            return createValue(static_cast<signed char>(0));
        case QMetaType::UChar:
            return createValue(static_cast<unsigned char>(0));
        case QMetaType::Short:
            return createValue(static_cast<short>(0));
        case QMetaType::UShort:
            return createValue(static_cast<unsigned short>(0));
        case QMetaType::Int:
            return createValue(static_cast<int>(0));
        case QMetaType::UInt:
            return createValue(static_cast<uint>(0));
        case QMetaType::Long:
            return createValue(static_cast<long>(0));
        case QMetaType::ULong:
            return createValue(static_cast<ulong>(0));
        case QMetaType::LongLong:
            return createValue(static_cast<qlonglong>(0));
        case QMetaType::ULongLong:
            return createValue(static_cast<qulonglong>(0));
        case QMetaType::Float:
            return createValue(static_cast<float>(0));
        case QMetaType::Double:
            return createValue(static_cast<double>(0));
        case QMetaType::QString:
            return createValue(QString());
        case QMetaType::QByteArray:
            return createValue(QByteArray());
        case QMetaType::QDate:
            return createValue(QDate());
        case QMetaType::QTime:
            return createValue(QTime());
        case QMetaType::QDateTime:
            return createValue(QDateTime());
        default:
            break;
        }

        if(typeName == "IString"){
            return createValue(IString());
        }else if(typeName == "std::string"){
            return createValue(std::string());
        }else if(typeName == "IStringView"){
            return createValue(IStringView());
        }else if(typeName == "IHttpMultiPart"){
            return const_cast<void*>(static_cast<const void*>(&IHttpMultiPart::Empty));
        }else if(typeName == "IHttpCookiePart"){
            return const_cast<void*>(static_cast<const void*>(&IHttpCookiePart::Empty));
        }

        IHttpAbort::abortTypeConvertionFailed("can not convert from IString to " + typeName.toQString(), $ISourceLocation);
        return nullptr;
    }

    static void* fromIJsonToPtr(const IJson& data, QMetaType::Type typeId, const IString& typeName)
    {
        switch (typeId) {
        case QMetaType::Bool:
            if(data.is_boolean()){
                return createValue(data.get<bool>());
            }
            if(data.is_number()){
                bool val = data.get<double>() != 0;
                return createValue(val);
            }
            if(data.is_array() || data.is_object()){
                return createValue(!data.empty());
            }
            if(data.is_string()){
                bool val = data.get<std::string>() == "true";
                return createValue(val);
            }
            return nullptr;
        case QMetaType::SChar:
            return convertFromIJsonToNumberPtr<signed char>(data);
        case QMetaType::UChar:
            return convertFromIJsonToNumberPtr<unsigned char>(data);
        case QMetaType::Short:
            return convertFromIJsonToNumberPtr<short>(data);
        case QMetaType::UShort:
            return convertFromIJsonToNumberPtr<ushort>(data);
        case QMetaType::Int:
            return convertFromIJsonToNumberPtr<int>(data);
        case QMetaType::UInt:
            return convertFromIJsonToNumberPtr<uint>(data);
        case QMetaType::Long:
            return convertFromIJsonToNumberPtr<long>(data);
        case QMetaType::ULong:
            return convertFromIJsonToNumberPtr<ulong>(data);
        case QMetaType::LongLong:
            return convertFromIJsonToNumberPtr<qlonglong>(data);
        case QMetaType::ULongLong:
            return convertFromIJsonToNumberPtr<qulonglong>(data);
        case QMetaType::Float:
            return convertFromIJsonToNumberPtr<float>(data);
        case QMetaType::Double:
            return convertFromIJsonToNumberPtr<double>(data);
        case QMetaType::QString:
            if(data.is_string()){
                return createValue(QString::fromStdString(data.get<std::string>()));
            }
            return nullptr;
        case QMetaType::QByteArray:
            if(data.is_string()){
                return createValue(QString::fromStdString(data.get<std::string>()));
            }
            return nullptr;
        case QMetaType::QDate:
            return convertFromIJsonToDatePtr<QDate>(data);
        case QMetaType::QTime:
            return convertFromIJsonToDatePtr<QTime>(data);
        case QMetaType::QDateTime:
            return convertFromIJsonToDatePtr<QDateTime>(data);
        default:
            break;
        }

        if(typeName == "IString"){
            if(data.is_string()){
                return createValue(IString(data.get<std::string>()));
            }
            return nullptr;
        }else if(typeName == "std::string"){
            if(data.is_string()){
                return createValue(data.get<std::string>());
            }
            return nullptr;
        }

        IHttpAbort::abortTypeConvertionFailed("can not convert from IJson to " + typeName.toQString(), $ISourceLocation);
        return nullptr;
    }

    static void* fromIStringToPtr(const IString& data, QMetaType::Type typeId, const IString& typeName)
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
        }

        IHttpAbort::abortTypeConvertionFailed("can not convert from IString to " + typeName.toQString(), $ISourceLocation);
        return nullptr;
    }

    static void* fromQVariantToPtr(const QVariant& value, QMetaType::Type typeId, const IString& typeName)
    {
        bool ok;
        switch (typeId) {
        case QMetaType::Bool:
            return createValue(value.toBool());
        case QMetaType::QString:
            return createValue(value.toString());
        case QMetaType::Short:{
            auto val = value.toInt(&ok);
            if(ok && val >= std::numeric_limits<short>::min() && val <= std::numeric_limits<short>::max()){
                return createValue(static_cast<short>(val));
            }
            return nullptr;
        }
        case QMetaType::UShort:{
            auto val = value.toUInt(&ok);
            if(ok && val >= std::numeric_limits<ushort>::min() && val <= std::numeric_limits<ushort>::max()){
                return createValue(static_cast<ushort>(val));
            }
            return nullptr;
        }
        case QMetaType::Int:
            value.toInt(&ok);
            return createValue(value.toInt(), ok);
        case QMetaType::UInt:
            value.toUInt(&ok);
            return createValue(value.toUInt(), ok);
        case QMetaType::Long:{
            if(sizeof(long) == sizeof(int)){
                value.toInt(&ok);
                return createValue(value.toInt(), ok);
            }else{
                value.toLongLong(&ok);
                return createValue(value.toLongLong(), ok);
            }
        }
        case QMetaType::ULong:{
            if(sizeof(unsigned long) == sizeof(unsigned int)){
                value.toUInt(&ok);
                return createValue(value.toUInt(), ok);
            }else{
                value.toULongLong(&ok);
                return createValue(value.toULongLong(), ok);
            }
        }
        case QMetaType::LongLong:
            value.toLongLong(&ok);
            return createValue(value.toLongLong(), ok);
        case QMetaType::ULongLong:
            value.toULongLong(&ok);
            return createValue(value.toULongLong(), ok);
        case QMetaType::Float:
            value.toFloat(&ok);
            return createValue(value.toFloat(), ok);
        case QMetaType::Double:
            value.toDouble(&ok);
            return createValue(value.toDouble(), ok);
        case QMetaType::QDate:{
            auto date = value.toDate();
            return createValue(date, date.isValid());
        }
        case QMetaType::QTime:{
            auto time = value.toTime();
            return createValue(time, time.isValid());
        }
        case QMetaType::QDateTime:{
            auto dateTime = value.toDateTime();
            return createValue(dateTime, dateTime.isValid());
        }
        default:
            break;
        }

        if(typeName == "IString"){
            return createValue(IString(value.toString()));
        }else if(typeName == "std::string"){
            return createValue(value.toString().toStdString());
        }

        IHttpAbort::abortTypeConvertionFailed("can not convert from QVariant to " + typeName.toQString(), $ISourceLocation);
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
        } else if(typeName == "IHttpMultiPart"){
            qFatal("this should never be called");
            if(ptr != &IHttpMultiPart::Empty){
                return delete static_cast<IHttpMultiPart*>(ptr);
            }
        }else if(typeName == "IHttpCookiePart"){
            if(ptr != &IHttpCookiePart::Empty){
                return delete static_cast<IHttpCookiePart*>(ptr);
            }
        }

        IHttpAbort::abortArgumentTypeError("error when delete refactored value. type:" + typeName.toQString());
    }
}

IHttpArgumentTypeDetail::IHttpArgumentTypeDetail(int typeId, QByteArray paramTypeName, QByteArray nameRaw)
    : IHttpArgumentType()
{
    m_typeId = QMetaType::Type(typeId);
    m_typeName = paramTypeName;
    m_nameRaw = nameRaw;
    resolveName();

    static QList<decltype(&IHttpArgumentTypeDetail::createEmbendTypes)> funs = {
        &IHttpArgumentTypeDetail::createEmbendTypes,
        &IHttpArgumentTypeDetail::createPartTypes,
        &IHttpArgumentTypeDetail::createDecorateTypes,
        &IHttpArgumentTypeDetail::createNormalTypes,
        &IHttpArgumentTypeDetail::createBeanTypes,
    };
    for(auto fun : funs){
        std::invoke(fun, this);
        if(m_createFun){
            return;
        }
    }

    QString tip = QString("parse argument type failed, this type and name can not be managed")
            .append(" TYPE: ").append(m_typeName.toQString())
            .append(" NAME: ").append(m_name.toQString());

    IHttpAbort::abortArgumentTypeError(tip, $ISourceLocation);
}

void IHttpArgumentTypeDetail::resolveName()
{
    static const QVector<IString> PREFIXES = {
        "auto", "PATH", "QUERY", "HEADER", "COOKIE", "SESSION", "BODY", "FORM", "JSON"
    };
    if(m_nameRaw.isEmpty()){
        IHttpAbort::abortArgumentTypeError("Http Method declaration should not only contain type, but also name. name can not be empty!");
    }

    // name
    IStringViewList args = m_nameRaw.split("_$");
    m_name = args.first();
    args.pop_front();

    // position
    if(args.length() == 0){
        return;
    }
    auto index = PREFIXES.indexOf(args.first());
    m_position = Position(index);
    args.pop_front();

    if(m_position == Position::Json){
        return;
    }

    // optional
    if(args.length() == 0){
        return;
    }
    if(args.first() != "OPTIONAL"){
        IHttpAbort::abortArgumentTypeError("two more restrict. currently we only support one POSITION restriction", $ISourceLocation);
    }
    m_optional = true;
    args.pop_front();

    if(args.length() == 0){         // default optional argument
        m_optionalPtr = detail::fromDefaultValueToPtr(m_typeId, m_typeName);
        if(m_optionalPtr == nullptr){
            qFatal("error");
        }
        if(!m_optionalPtr){
            QString tip = QString("optional argument without default value can not be created. ")
                    .append(" TYPE: ").append(m_typeName.toQString())
                    .append(" NAME: ").append(m_name.toQString());
            IHttpAbort::abortArgumentTypeError(tip, $ISourceLocation);
        }
        return;
    }

    m_optionalString = args.first();        // giving optional argument
    if(detail::isTypeConvertable(m_typeId, m_typeName)){
        m_optionalPtr = detail::fromIStringToPtr(m_optionalString, m_typeId, m_typeName);
        if(!m_optionalPtr){
            QString tip = QString("optional argument default value can not be created. ")
                    .append(" TYPE: ").append(m_typeName.toQString())
                    .append(" NAME: ").append(m_name.toQString())
                    .append(" DEFAULT VALUE: ").append(m_optionalString.toQString());
            IHttpAbort::abortArgumentTypeError(tip, $ISourceLocation);
        }
    }else{
        IHttpAbort::abortArgumentTypeError("optional not convertable", $ISourceLocation);
    }
}

void IHttpArgumentTypeDetail::createEmbendTypes()
{
    static QList<decltype(&IHttpArgumentTypeDetail::createRequestType)> funs = {
        &IHttpArgumentTypeDetail::createRequestType,
        &IHttpArgumentTypeDetail::createResponseType,
        &IHttpArgumentTypeDetail::createMultiPartJarType,
        &IHttpArgumentTypeDetail::createCookieJarType,
        &IHttpArgumentTypeDetail::createSessionJarType,
        &IHttpArgumentTypeDetail::createHeaderJarType,
    };

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
    static QList<CreateFun> funs = {
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
    if(m_optional){
        IHttpAbort::abortArgumentTypeError("optional should not exist when data type is IHttpMultiPart", $ISourceLocation);
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
    if(m_optional){
        IHttpAbort::abortArgumentTypeError("optional should not exist when data type is IHttpCookiePart", $ISourceLocation);
    }

    auto self = *this;
    this->m_createFun = [=](IRequest& request) -> void*{
        if(request.impl().m_reqRaw.m_cookies.contains(self.m_name)){
            const auto& value = request.impl().m_reqRaw.m_cookies.value(self.m_name);
            return new IHttpCookiePart(self.m_name, value);
        }
        request.setInvalid(IHttpInternalErrorInvalid("cookie not exist"));
        return nullptr;
    };

    this->m_destroyFun = [=](void* ptr){
        if(ptr){
            delete static_cast<IHttpCookiePart*>(ptr);
        }
    };
}

void IHttpArgumentTypeDetail::createDecorateTypes()
{
    QList<CreateFun> funs = {
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
        if(req.impl().m_reqRaw.m_queries.contains(self.m_name)){
            auto data = req.impl().m_reqRaw.m_queries[self.m_name];
            auto ptr = detail::fromIStringToPtr(data, self.m_typeId, self.m_typeName);
            if(!ptr){
                req.setInvalid(IHttpBadRequestInvalid("query field value not proper"));
            }
            return ptr;
        }
        if(self.m_optional){
            return self.m_optionalPtr;
        }
        req.setInvalid(IHttpInternalErrorInvalid("query argument not found"));
        return nullptr;
    };

    this->m_destroyFun = [=](void* ptr){
        if(ptr && ptr != self.m_optionalPtr){
            detail::deletePtr(ptr, self.m_typeId, self.m_typeName);
        }
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
                auto ptr = detail::fromIStringToPtr(value, self.m_typeId, self.m_typeName);
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

    this->m_destroyFun = [=](void* ptr){
        if(ptr && ptr != self.m_optionalPtr){
            return detail::deletePtr(ptr, self.m_typeId, self.m_typeName);
        }
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
        if(detail::isStringListType(self.m_typeId, self.m_typeName)){       // process stringlist
            auto values = request.impl().m_reqRaw.m_cookies.values(self.m_name);
            auto ptr = detail::fromIStringViewListToPtr(values, self.m_typeId, self.m_typeName);
            if(!ptr){
                request.setInvalid(IHttpBadRequestInvalid("header field value not proper"));
            }
            return ptr;
        }

        //normal cookies
        if(request.impl().m_reqRaw.m_cookies.contains(self.m_name)){
            auto value = request.impl().m_reqRaw.m_cookies.value(self.m_name);
            auto ptr = detail::fromIStringToPtr(value, self.m_typeId, self.m_typeName);
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
    };

    this->m_destroyFun = [=](void* ptr){
        if(ptr && ptr != self.m_optionalPtr){
            return detail::deletePtr(ptr, self.m_typeId, self.m_typeName);
        }
    };
}

void IHttpArgumentTypeDetail::createSessionType()
{
    if(this->m_position != Position::Session){
        return;
    }

    auto self = *this;
    this->m_createFun = [=](IRequest& req) -> void*{
        if(req.isSessionExist()){
            auto value = req.session().getValue(self.m_name.toQString());
            if(value.isValid()){
                auto ptr = detail::fromQVariantToPtr(value, self.m_typeId, self.m_typeName);
                if(!ptr){
                    req.setInvalid(IHttpBadRequestInvalid("session field value not proper"));
                }
                return ptr;
            }
        }

        if(self.m_optional){
            return self.m_optionalPtr;
        }

        req.setInvalid(IHttpInternalErrorInvalid("session field not found"));
        return nullptr;
    };

    this->m_destroyFun = [=](void* ptr){
        if(ptr && ptr != self.m_optionalPtr){
            return detail::deletePtr(ptr, self.m_typeId, self.m_typeName);
        }
    };
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
        auto ptr = detail::fromIStringToPtr(value, self.m_typeId, self.m_typeName);
        if(!ptr){
            req.setInvalid(IHttpBadRequestInvalid("path value can not be converted"));
        }
        return ptr;
    };

    this->m_destroyFun = [=](void* ptr){
        detail::deletePtr(ptr, self.m_typeId, self.m_typeName);
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

    auto self = *this;
    this->m_createFun = [=](IRequest& req)->void*{
        if(req.impl().m_reqRaw.m_contentLength == 0 && req.impl().m_reqRaw.m_isChunked==false){
            if(self.m_optional){
                return self.m_optionalPtr;
            }
            req.setInvalid(IHttpInternalErrorInvalid("request do not contain body"));
            return nullptr;
        }
        auto ptr = detail::fromIStringToPtr(req.impl().m_reqRaw.m_body, self.m_typeId, self.m_typeName);
        if(!ptr){
            req.setInvalid(IHttpBadRequestInvalid("body field can not be conveted"));
        }
        return ptr;
    };

    this->m_destroyFun = [=](void* ptr){
        if(ptr && ptr != self.m_optionalPtr){
            detail::deletePtr(ptr, self.m_typeId, self.m_typeName);
        }
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

        if(req.impl().m_reqRaw.m_forms.contains(self.m_name)){
            auto data = req.impl().m_reqRaw.m_forms[self.m_name];
            auto ptr = detail::fromIStringToPtr(data, self.m_typeId, self.m_typeName);
            if(!ptr){
                req.setInvalid(IHttpBadRequestInvalid("form field value not proper"));
            }
            return ptr;
        }
        if(self.m_optional){
            return self.m_optionalPtr;
        }
        req.setInvalid(IHttpInternalErrorInvalid("not found form value"));
        return nullptr;
    };

    this->m_destroyFun = [=](void* ptr){
        if(ptr && ptr != self.m_optionalPtr){
            detail::deletePtr(ptr, self.m_typeId, self.m_typeName);
        }
    };
}

// NOTE: optional in latter time
void IHttpArgumentTypeDetail::createJsonType()
{
    if(m_position != Position::Json){
        return;
    }
    if(!detail::isTypeConvertable(m_typeId, m_typeName) && m_typeName != "IJson"){
        QString tip = QString("$Json decorator must use IJson and other base type!")
                .append(" TYPE: ").append(m_typeName.toQString())
                .append(" NAME: ").append(m_nameRaw.toQString());
        IHttpAbort::abortArgumentTypeError(tip, $ISourceLocation);
    }
    if(m_optional){
        IHttpAbort::abortArgumentTypeError("IJson can not be optional currently", $ISourceLocation);
    }

    auto self = *this;
    auto pointer = createJsonPointer(m_nameRaw);
    this->m_createFun = [=](IRequest& request)->void*{
        if(request.mime() != IHttpMime::APPLICATION_JSON && request.mime() != IHttpMime::APPLICATION_JSON_UTF8){
            request.setInvalid(IHttpInternalErrorInvalid("request do not contain json payload"));
            return nullptr;
        }
        const auto& json = request.impl().m_reqRaw.m_json;
        if(json.contains(pointer)){
            if(self.m_typeName == "IJson"){
                return new IJson(json[pointer]);
            }
            auto ptr = detail::fromIJsonToPtr(json[pointer], self.m_typeId, self.m_typeName);
            if(!ptr){
                request.setInvalid(IHttpBadRequestInvalid("json field value not proper"));
            }
            return ptr;
        }
        request.setInvalid(IHttpInternalErrorInvalid("not found json body"));
        return nullptr;
    };
    this->m_destroyFun = [=](void* ptr){
        if(self.m_position == Position::Json){
            if(self.m_typeName == "IJson"){
                delete static_cast<IJson*>(ptr);
            }else{
                detail::deletePtr(ptr, self.m_typeId, self.m_typeName);
            }
        }
    };
}

void IHttpArgumentTypeDetail::createNormalTypes()
{
    QList<CreateFun> funs = {
        &IHttpArgumentTypeDetail::createNormalJsonType,
        &IHttpArgumentTypeDetail::createNormalMixedType,
    };

    for(auto fun : funs){
        std::invoke(fun, this);
        if(m_createFun){
            return;
        }
    }
}

void IHttpArgumentTypeDetail::createNormalJsonType()
{
    if(m_position != Position::Auto){
        return;
    }
    static const auto types = makeTypes("IJson");
    if(!types.contains(m_typeName)){
        return;
    }

    auto self = *this;
    this->m_createFun = [=](IRequest& request)->void*{
        if(request.mime() != IHttpMime::APPLICATION_JSON && request.mime() != IHttpMime::APPLICATION_JSON_UTF8){
            request.setInvalid(IHttpInternalErrorInvalid("request do not contain json payload"));
            return nullptr;
        }

        const auto& json = request.impl().m_reqRaw.m_json;
        return const_cast<IJson*>(&json);
    };
}

void IHttpArgumentTypeDetail::createNormalMixedType()
{
    if(m_position != Position::Auto){
        return;
    }
    if(!detail::isTypeConvertable(m_typeId, m_typeName)){
        return;
    }

    auto self = *this;
    this->m_createFun = [=](IRequest& req) -> void* {
        while(true){
            // form
            if(req.mime() == IHttpMime::APPLICATION_WWW_FORM_URLENCODED
                    && req.bodyFormParameters().contains(self.m_name)){
                auto data = req.bodyFormParameters()[self.m_name];
                auto ptr = detail::fromIStringToPtr(data, self.m_typeId, self.m_typeName);
                if(!ptr){
                    req.setInvalid(IHttpBadRequestInvalid("form field value not proper"));
                }
                return ptr;
            }

            // multi part
            if(req.mime() == IHttpMime::MULTIPART_FORM_DATA
                    && req.multiPartJar().contain(self.m_name)){
                const auto& value = req.multiPartJar().getMultiPart(self.m_name).m_content;
                auto ptr = detail::fromIStringToPtr(value, self.m_typeId, self.m_typeName);
                if(!ptr){
                    req.setInvalid(IHttpBadRequestInvalid("multipart field value not proper"));
                }
                return ptr;
            }
            // json
            auto pathStr = QString("/").append(self.m_name.toQString()).toStdString();
            IJson::json_pointer path(pathStr);
            if((req.mime() != IHttpMime::APPLICATION_JSON && req.mime() != IHttpMime::APPLICATION_JSON_UTF8)
                    && req.impl().m_reqRaw.m_json.contains(path)){
                return detail::fromIJsonToPtr(req.impl().m_reqRaw.m_json[path], self.m_typeId, self.m_typeName);
            }
            // query
            if(req.queryParameters().contains(self.m_name)){
                auto value = req.queryParameters().value(self.m_name);
                auto ptr = detail::fromIStringToPtr(value, self.m_typeId, self.m_typeName);
                if(!ptr){
                    req.setInvalid(IHttpBadRequestInvalid("query field value not proper"));
                }
                return ptr;
            }
            // path
            if(req.pathParameters().contains(self.m_name)){
                auto value =  req.pathParameters().value(self.m_name);
                auto ptr = detail::fromIStringToPtr(value, self.m_typeId, self.m_typeName);
                if(!ptr){
                    req.setInvalid(IHttpBadRequestInvalid("path field value not proper"));
                }
                return ptr;
            }
            // cookie
            if(req.cookieJar().containRequestCookieKey(self.m_name)){
                auto value = req.cookieJar().getRequestCookie(self.m_name).m_value;
                auto ptr = detail::fromIStringToPtr(value, self.m_typeId, self.m_typeName);
                if(!ptr){
                    req.setInvalid(IHttpBadRequestInvalid("cookie field value not proper"));
                }
                return ptr;
            }
            // session
            if(req.session().contains(self.m_name.toQString())){
                auto value = req.session().getValue(self.m_name.toQString());
                auto ptr = detail::fromQVariantToPtr(value, self.m_typeId, self.m_typeName);
                if(!ptr){
                    req.setInvalid(IHttpBadRequestInvalid("session field value not proper"));
                }
                return ptr;
            }
            // header
            if(req.headerJar().containRequestHeaderKey(self.m_name)){
                auto value = req.headerJar().getRequestHeaderValue(self.m_name);
                auto ptr = detail::fromIStringToPtr(value, self.m_typeId, self.m_typeName);
                if(!ptr){
                    req.setInvalid(IHttpBadRequestInvalid("header field value not proper"));
                }
                return ptr;
            }
        }

        req.setInvalid(IHttpInternalErrorInvalid("parameter not found"));
        return nullptr;
    };

    this->m_destroyFun = [=](void* ptr){
        if(ptr){
            return detail::deletePtr(ptr, self.m_typeId, self.m_typeName);
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

        auto ptr = QMetaType(self.m_typeId).create();
        auto fun = IBeanTypeManage::instance().getBeanFromJsonFun(self.m_typeId);
        if(!fun ||  !fun(ptr, req.bodyJson())){
            req.setInvalid(IHttpInternalErrorInvalid("json can not be converted to Bean"));
        }
        return ptr;
    };
    this->m_destroyFun = [=](void* ptr){
        if(ptr){
            QMetaType(self.m_typeId).destroy(ptr);
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
    auto args = data.toQString().split("_$");
    if(args.length()>1){
        args.takeAt(1);
    }
    auto path = args.join("/").prepend("/");
    IJson::json_pointer pointer(path.toStdString());
    return pointer;
}

namespace ISpawnUtil
{
    template<>
    IHttpArgumentType construct(int paramTypeId, QByteArray paramTypeName, QByteArray paramName)
    {
        return IHttpArgumentTypeDetail(paramTypeId, paramTypeName, paramName);
    }
}

$PackageWebCoreEnd
