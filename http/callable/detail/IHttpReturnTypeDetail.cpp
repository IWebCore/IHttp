#include "IHttpReturnTypeDetail.h"
#include "core/util/ISpawnUtil.h"
#include "core/util/IStringUtil.h"
#include "core/bean/IBeanTypeManage.h"
#include "core/config/IProfileImport.h"
#include "http/callable/IHttpReturnType.h"
#include "http/IRequest.h"
#include "http/IResponse.h"
#include "http/IHttpAbort.h"
#include "http/invalid/IHttpBadRequestInvalid.h"
#include "http/invalid/IHttpInternalErrorInvalid.h"
#include "http/response/IJsonResponse.h"
#include "http/response/IBytesResponse.h"
#include "http/response/IPlainTextResponse.h"
#include "http/response/IFileResponse.h"
#include "http/response/IHttpResponseWare.h"
#include "http/detail/IHttpRequestImpl.h"

$PackageWebCoreBegin

IHttpReturnTypeDetail::IHttpReturnTypeDetail(QMetaType::Type type, const std::string& name)
    : IHttpReturnType()
{
    m_resolveFunction = nullptr;
    m_typeId = type;
    m_typeName = name;
    createResolveFuntion();
}

// 另外，之后考虑用户可以自定义返回类型。
void IHttpReturnTypeDetail::createResolveFuntion()
{
    QVector<decltype(&IHttpReturnTypeDetail::createResponseFun)> funs = {
            &IHttpReturnTypeDetail::createResponseFun,
            &IHttpReturnTypeDetail::createVoidFun,
            &IHttpReturnTypeDetail::createBeanFun,
            &IHttpReturnTypeDetail::createStatusFun,
            &IHttpReturnTypeDetail::createStdStringFun,
            &IHttpReturnTypeDetail::createQStringFun,
            &IHttpReturnTypeDetail::createIStringFun,
            &IHttpReturnTypeDetail::createQByteArrayFun,
            &IHttpReturnTypeDetail::createQStringListFun,
            &IHttpReturnTypeDetail::createIJsonFun,

            &IHttpReturnTypeDetail::createQDateFun,
            &IHttpReturnTypeDetail::createQTimeFun,
            &IHttpReturnTypeDetail::createQDateTimeFun,

            &IHttpReturnTypeDetail::createNumberFun,

    };
    for(auto fun : funs){
        std::invoke(fun, this);
        if(m_resolveFunction){
            break;
        }
    }
    if(!m_resolveFunction){
        QString tip = "return type can not be resolved by system.";
        tip.append(" Type: ").append(QString::fromStdString(m_typeName));
        IHttpAbort::abortHttpCallableError(tip, $ISourceLocation);
    }
}

void IHttpReturnTypeDetail::createResponseFun()
{
    if(IHttpResponseManage::instance().containResponse(m_typeName)){
        m_resolveFunction = [](IHttpRequestImpl& impl, void* ptr){
            impl.setResponseWare(*static_cast<IHttpResponseWare*>(ptr));
        };
    }
}

void IHttpReturnTypeDetail::createBeanFun()
{
    if(IBeanTypeManage::instance().isBeanIdExist(m_typeId)){
        auto self = *this;
        m_resolveFunction = [self](IHttpRequestImpl& impl, void* ptr){
            IJson json = IBeanTypeManage::instance().getBeanToJsonFun(self.m_typeId)(ptr);
            impl.setResponseWare(IJsonResponse(std::move(json)));
        };
    }
}

void IHttpReturnTypeDetail::createVoidFun()
{
    if(m_typeId == QMetaType::Void){
        m_resolveFunction = [](IHttpRequestImpl& impl, void*){
            if(impl.m_respRaw.m_mime == IHttpMimeUtil::MIME_UNKNOWN_STRING){
                impl.m_respRaw.setMime(IHttpMime::TEXT_PLAIN_UTF8);
            }
            if(impl.m_respRaw.m_status == IHttpStatus::UNKNOWN){
                impl.m_respRaw.m_status = IHttpStatus::OK_200;
            }
        };
    }
}

void IHttpReturnTypeDetail::createStatusFun()
{
    if(m_typeName == "IHttpStatus"){
        m_resolveFunction = [](IHttpRequestImpl& impl, void* ptr){
            impl.setResponseWare(IStatusResponse(*static_cast<int*>(ptr)));
        };
    }
}

void IHttpReturnTypeDetail::createStdStringFun()
{
    if(m_typeId == qRegisterMetaType<std::string>() && m_typeName == "std::string"){
        m_resolveFunction = [](IHttpRequestImpl& impl, void* ptr){
            const std::string& value = *static_cast<std::string*>(ptr);
            if(IStringUtil::startsWith(value, "$")){
                IHttpResponseWare* ware = IHttpResponseManage::instance().convertableMatch(value);
                if(ware){
                    auto content = ware->prefixCreate(value);
                    impl.setResponseWare(*content);
                    delete content;
                    return;
                }
            }
            setTextResponse(impl, std::string(value));
        };
    }
}

void IHttpReturnTypeDetail::createQStringFun()
{
    if(m_typeId == QMetaType::QString){
        m_resolveFunction = [](IHttpRequestImpl& impl, void* ptr){
            std::string value = static_cast<QString*>(ptr)->toStdString();
            if(IStringUtil::startsWith(value, "$")){
                IHttpResponseWare* ware = IHttpResponseManage::instance().convertableMatch(value);
                if(ware){
                    auto content = ware->prefixCreate(value);
                    impl.setResponseWare(*content);
                    delete content;
                    return;
                }
            }
            setTextResponse(impl, std::move(value));
        };
    }
}

void IHttpReturnTypeDetail::createQByteArrayFun()
{
    if(m_typeId == QMetaType::QByteArray){
        m_resolveFunction = [](IHttpRequestImpl& impl, void *ptr){
            QByteArray& array = *static_cast<QByteArray*>(ptr);
            impl.setResponseWare(IBytesResponse(std::move(array)));
        };
    }
}

void IHttpReturnTypeDetail::createIStringFun()
{
    if(m_typeId == qRegisterMetaType<IString>() && m_typeName == "IString"){
        m_resolveFunction = [](IHttpRequestImpl& impl, void* ptr){
            std::string value = static_cast<IString*>(ptr)->toStdString();
            if(IStringUtil::startsWith(value, "$")){
                IHttpResponseWare* ware = IHttpResponseManage::instance().convertableMatch(value);
                if(ware){
                    auto content = ware->prefixCreate(value);
                    impl.setResponseWare(*content);
                    delete content;
                    return;
                }
            }
            setTextResponse(impl, std::move(value));
        };
    }
}

void IHttpReturnTypeDetail::createQStringListFun()
{
    if(m_typeId == QMetaType::QStringList){
        m_resolveFunction = [](IHttpRequestImpl& impl, void* ptr){
            IJson json = IJson::array();
            const QStringList& values = *static_cast<QStringList*>(ptr);
            for(const QString& val : values){
                json.push_back(val.toStdString());
            }
            impl.setResponseWare(IJsonResponse(json));
        };
    }
}

void IHttpReturnTypeDetail::createIJsonFun()
{
    if(m_typeId == qRegisterMetaType<IJson>() && m_typeName == "IJson"){
        m_resolveFunction = [](IHttpRequestImpl& impl, void* ptr){
            impl.setResponseWare(IJsonResponse(*static_cast<IJson*>(ptr)));
        };
    }
}

void IHttpReturnTypeDetail::createQDateFun()
{
    if(m_typeId == QMetaType::QDate){
        m_resolveFunction = [](IHttpRequestImpl& impl, void* ptr){
            auto date = static_cast<QDate*>(ptr);
            if(!date->isValid()){
                impl.setInvalid(IHttpInternalErrorInvalid());
                return;
            }

            static const $QString format{"/http/QDateFormat", "yyyy-MM-dd"};
            impl.setResponseWare(IPlainTextResponse(date->toString(*format)));
        };
    }
}

void IHttpReturnTypeDetail::createQDateTimeFun()
{
    if(m_typeId == QMetaType::QDateTime){
        m_resolveFunction = [](IHttpRequestImpl& impl, void* ptr){
            auto dateTime = static_cast<QDateTime*>(ptr);
            if(!dateTime->isValid()){
                impl.setInvalid(IHttpInternalErrorInvalid());
                return;
            }

            static const $QString format{"/http/QDateTimeFormat", "yyyy-MM-ddTHH:mm:ss"};
            impl.setResponseWare(IPlainTextResponse(dateTime->toString(*format)));
        };
    }
}

void IHttpReturnTypeDetail::createQTimeFun()
{
    if(m_typeId == QMetaType::QTime){
        m_resolveFunction = [](IHttpRequestImpl& impl, void* ptr){
            auto time = static_cast<QTime*>(ptr);
            if(!time->isValid()){
                impl.setInvalid(IHttpInternalErrorInvalid());
                return;
            }

            static const $QString format{"/http/QTimeFormat", "HH:mm:ss"};
            impl.setResponseWare(IPlainTextResponse(time->toString(*format)));
        };
    }
}

void IHttpReturnTypeDetail::createNumberFun()
{
    if(m_typeId == QMetaType::Short){
        m_resolveFunction = [](IHttpRequestImpl& impl, void* ptr){
            impl.setResponseWare(IPlainTextResponse(std::to_string(*static_cast<short*>(ptr))));
        };
        return;
    }
    if(m_typeId == QMetaType::UShort){
        m_resolveFunction = [](IHttpRequestImpl& impl, void* ptr){
            impl.setResponseWare(IPlainTextResponse(std::to_string(*static_cast<ushort*>(ptr))));
        };
        return;
    }
    if(m_typeId == QMetaType::Int){
        m_resolveFunction = [](IHttpRequestImpl& impl, void* ptr){
            impl.setResponseWare(IPlainTextResponse(std::to_string(*static_cast<int*>(ptr))));
        };
        return;
    }
    if(m_typeId == QMetaType::UInt){
        m_resolveFunction = [](IHttpRequestImpl& impl, void* ptr){
            impl.setResponseWare(IPlainTextResponse(std::to_string(*static_cast<uint*>(ptr))));
        };
        return;
    }
    if(m_typeId == QMetaType::Long){
        m_resolveFunction = [](IHttpRequestImpl& impl, void* ptr){
            impl.setResponseWare(IPlainTextResponse(std::to_string(*static_cast<long*>(ptr))));
        };
        return;
    }
    if(m_typeId == QMetaType::ULong){
        m_resolveFunction = [](IHttpRequestImpl& impl, void* ptr){
            impl.setResponseWare(IPlainTextResponse(std::to_string(*static_cast<ulong*>(ptr))));
        };
        return;
    }
    if(m_typeId == QMetaType::LongLong){
        m_resolveFunction = [](IHttpRequestImpl& impl, void* ptr){
            impl.setResponseWare(IPlainTextResponse(std::to_string(*static_cast<qlonglong*>(ptr))));
        };
        return;
    }
    if(m_typeId == QMetaType::ULongLong){
        m_resolveFunction = [](IHttpRequestImpl& impl, void* ptr){
            impl.setResponseWare(IPlainTextResponse(std::to_string(*static_cast<qulonglong*>(ptr))));
        };
        return;
    }
    if(m_typeId == QMetaType::Float){
        m_resolveFunction = [](IHttpRequestImpl& impl, void* ptr){
            impl.setResponseWare(IPlainTextResponse(std::to_string(*static_cast<float*>(ptr))));
        };
        return;
    }
    if(m_typeId == QMetaType::Double){
        m_resolveFunction = [](IHttpRequestImpl& impl, void* ptr){
            impl.setResponseWare(IPlainTextResponse(std::to_string(*static_cast<double*>(ptr))));
        };
        return;
    }
}

void IHttpReturnTypeDetail::setTextResponse(IHttpRequestImpl &impl, std::string &&text)
{
    impl.m_respRaw.setMime(IHttpMime::TEXT_PLAIN_UTF8);
    impl.m_respRaw.setContent(new IHttpResponseContent(std::move(text)));

}

namespace ISpawnUtil
{
    template<>
    IHttpReturnType construct(int type, const char* name)
    {
        return IHttpReturnTypeDetail(QMetaType::Type(type), name);
    }
}

$PackageWebCoreEnd
