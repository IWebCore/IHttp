#include "IHttpCallable.h"

#include "core/util/IPackageUtil.h"
#include "core/util/ISpawnUtil.h"
#include "core/abort/IAbortInterface.h"

$PackageWebCoreBegin

class IHttpCallableDetail : public IHttpCallable
{
public:
    IHttpCallableDetail(void *handler, const QString &className, const QMetaMethod &metaMethod);

public:
    void createReturnNode();
    void createArgumentNodes();
    void createSignature();

private:
    void checkMethodOfReturnVoid();
};

IHttpCallableDetail::IHttpCallableDetail(void *handler, const QString &className, const QMetaMethod &method)
    : IHttpCallable()
{
    m_handler = handler;
    m_className = className;
    m_metaMethod = method;
    m_metaCall = static_cast<IGadgetUnit*>(handler)->staticMetaCallFunction();

    createReturnNode();
    createArgumentNodes();
    createSignature();

    checkMethodOfReturnVoid();
}

void IHttpCallableDetail::createReturnNode()
{
    m_returnNode = ISpawnUtil::construct<IHttpReturnType>(m_metaMethod.returnType(), m_metaMethod.typeName());
}

void IHttpCallableDetail::createArgumentNodes()
{
    auto names = m_metaMethod.parameterNames();
    auto types = m_metaMethod.parameterTypes();

    for(int i=0;i<m_metaMethod.parameterCount(); i++){
        auto id = m_metaMethod.parameterType(i);
        m_argumentNodes.append(ISpawnUtil::construct<IHttpArgumentType>(id, types[i], names[i]));
    }
}

void IHttpCallableDetail::createSignature()
{
    QStringList args;
    for(const IHttpArgumentType& node : m_argumentNodes){
        args.append(node.m_typeName.toQString() + " " + node.m_name.toQString());
    }

    m_signature.append(QString::fromStdString(m_returnNode.m_typeName)).append(' ')
            .append(m_className).append("::").append(m_metaMethod.name()).append("(")
            .append(args.join(", ")).append(")");
}

void IHttpCallableDetail::checkMethodOfReturnVoid()
{
//    if(returnNode.typeId != QMetaType::Void){
//        return;
//    }
//    static const QStringList s_nodeNames ={
//        "IResponse", "IResponse&", "IRequest", "IRequest&"
//    };
//    for(const IArgumentType& info : argumentNodes){
//        if(s_nodeNames.contains(info.typeName)){
//            return;
//        }
//    }

//    QString info = "mapping function that return void should include IResponse in side function parameters\n"
//                   "at Function : " + signature;
//    qFatal(info.toUtf8());
}

namespace ISpawnUtil
{
    template<>
    IHttpCallable construct(void *handler, QString className, QMetaMethod method)
    {
        return IHttpCallableDetail(handler, className, method);
    }
}

$PackageWebCoreEnd
