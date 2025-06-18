#include "IHttpControllerCallableInfo.h"
#include "core/util/ISpawnUtil.h"
#include "http/IHttpAbort.h"
#include "http/biscuits/IHttpMethod.h"
#include "http/controller/IHttpControllerAbort.h"
#include "http/controller/IHttpControllerActionMapping.h"
#include "http/path/IHttpPathFragment.h"

$PackageWebCoreBegin

namespace detail
{
    static constexpr char CONTROLLER_CALLABLE_MAPPING_PREFIX[] = "IHttpControllerCallableMapping$$$";
    static constexpr char CONTROLLER_CALLABLE_PROPERTY_PREFIX [] = "IHttpControllerCallableProperty$$$";
    static constexpr char CONTROLLER_MAPPING_FLAG[] = "IHttpControllerMapping$";
}

IHttpCallableMappingInfo::IHttpCallableMappingInfo(const QString &key)
{
    auto args = key.split("$$$");
    if(args.length() != 4){
        qFatal("invalid split");
    }

    this->m_index = args.last().toInt();
    args.pop_back();

    this->m_method = IHttpMethodUtil::toMethod(args.last());
    args.pop_back();

    this->m_funName = args.last();
}

IHttpControllerCallableInfo::IHttpControllerCallableInfo(void* handler_, const QString &className_, const QMap<QString, QString> &classInfo_, const QVector<QMetaMethod> &methods_)
{
    this->m_handler = handler_;
    this->m_className = className_;
    this->m_classInfos = classInfo_;
    this->m_classMethods = methods_;

    parseRootPaths();
    parseMapppingInfos();
    parseToActions();
    findActionsProperties();

    checkPathArgumentMatch();
    checkMappingOverloadFunctions();
    checkMappingNameAndFunctionIsMatch();
}

void IHttpControllerCallableInfo::parseMapppingInfos()
{
    auto keys = m_classInfos.keys();
    for(auto key : keys){
        if(key.startsWith(detail::CONTROLLER_CALLABLE_MAPPING_PREFIX)){
            IHttpCallableMappingInfo info(key);
            info.m_fragments = rootFragments;
            auto fragments = parseFragments(m_classInfos[key]);
            info.m_fragments.insert(info.m_fragments.end(), fragments.begin(), fragments.end());
            m_mappingInfos.append(info);
        }
    }
}

void IHttpControllerCallableInfo::parseToActions()
{
    for(const IHttpCallableMappingInfo& mapping : m_mappingInfos){
        IHttpControllerAction node;
        node.m_httpMethod = mapping.m_method;
        node.m_path = ISpawnUtil::construct<IHttpPath, const std::vector<IHttpPathFragment>&>(mapping.m_fragments);
        node.m_callable = getHttpMethodNode(mapping.m_funName);
        m_actionNodes.append(node);
    }
}

void IHttpControllerCallableInfo::findActionsProperties()
{
    auto keys = m_classInfos.keys();
    for(IHttpControllerAction& action : m_actionNodes){
        QString methodName = action.m_callable.m_metaMethod.name();
        QString prefix = QString(detail::CONTROLLER_CALLABLE_PROPERTY_PREFIX).append(methodName).append("$$$");
        for(auto info : keys){
            if(info.startsWith(prefix)){
                auto name = info.mid(prefix.length()).split("$$$").first();
                action.setProperty(name, m_classInfos[info]);
            }
        }
    }
}

void IHttpControllerCallableInfo::parseRootPaths()
{
    if(m_classInfos.contains(detail::CONTROLLER_MAPPING_FLAG) && !m_classInfos[detail::CONTROLLER_MAPPING_FLAG].isEmpty()){
        rootFragments = parseFragments(m_classInfos[detail::CONTROLLER_MAPPING_FLAG]);
    }
}

std::vector<IHttpPathFragment> IHttpControllerCallableInfo::parseFragments(const QString &path)
{
    std::vector<IHttpPathFragment> ret;
    auto args =  path.split("/");
    for(const QString& arg : args){
        auto piece = arg.trimmed();
        if(piece == "." || piece == ".."){
            IHttpControllerAbort::abortUrlDotAndDotDotError("Controller mapping:" + m_classInfos[detail::CONTROLLER_MAPPING_FLAG]);
        }
        if(!piece.isEmpty()){
            ret.push_back(ISpawnUtil::construct<IHttpPathFragment>(piece));
        }
    }
    return ret;
}

IHttpCallable IHttpControllerCallableInfo::getHttpMethodNode(const QString &name)
{
    for(const QMetaMethod& method : m_classMethods){
        if(name == method.name()){
            return ISpawnUtil::construct<IHttpCallable>(m_handler, m_className, method);
        }
    }
    QString tip = QString("CLASS: ").append(m_className).append(" FUNCTION NAME: ").append(name);
    IHttpAbort::abortMappingAndFunctionMismatch(tip, $ISourceLocation);
    return {};
}

void IHttpControllerCallableInfo::checkPathArgumentMatch()
{
    for(const IHttpControllerAction& node : m_actionNodes){
        auto keys = node.m_path.m_actionNameMap.keys();
        for(const IHttpArgumentType& arg : node.m_callable.m_argumentNodes){
            if(!arg.m_name.isEmpty() && arg.m_position == IHttpArgumentType::Path){
                if(!keys.contains(arg.m_name)){
                    qFatal("error, path argument not match");
                }
            }
        }
    }
}

void IHttpControllerCallableInfo::checkMappingOverloadFunctions()
{
    QSet<QString> usedMethods;
    for(const IHttpCallableMappingInfo& info : m_mappingInfos){
        usedMethods.insert(info.m_funName);
    }

    QStringList names;
    for(const auto& method : m_classMethods){
        if(usedMethods.contains(method.name())){
            if(names.contains(method.name())){
                IHttpControllerAbort::abortOverloadOrDefaultValueFunctionNotSupported(QString("duplicated overloaded function name: ").append(method.name()));
            }
            names.append(method.name());
        }
    }
}

void IHttpControllerCallableInfo::checkMappingNameAndFunctionIsMatch()
{
    QSet<QString> infoNames;
    for(const auto& info : m_mappingInfos){
        infoNames.insert(info.m_funName);
    }

    QSet<QString> methodNames;
    for(const auto& method : m_classMethods){
        methodNames.insert(method.name());
    }

    for(const auto& name : infoNames){
        if(!methodNames.contains(name)){
            IHttpControllerAbort::abortMappingMismatchFatal(
                        QString("required function name: ").append(name), $ISourceLocation);
        }
    }
}

namespace ISpawnUtil
{
    template<>
    void construct<void, void*, const QString&, const QMap<QString, QString> &, const QVector<QMetaMethod>&>(
        void *handler,
        const QString &className,
        const QMap<QString, QString> &classInfo,
        const QVector<QMetaMethod> &methods
    ){
        IHttpControllerCallableInfo info(handler, className, classInfo, methods);
        const auto& actions = info.m_actionNodes;
        for(const IHttpControllerAction& action : actions){
            IHttpControllerActionMapping::instance().registerUrlActionNode(action);
        }
    }

}

$PackageWebCoreEnd
