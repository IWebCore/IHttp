#pragma once

#include "http/controller/IHttpControllerAction.h"

$PackageWebCoreBegin

struct IHttpCallableMappingInfo
{
public:
    IHttpCallableMappingInfo(const QString& key);

public:
    int m_index;
    QString m_funName;
    std::vector<IHttpPathFragment> m_fragments;
    IHttpMethod m_method;
};

class IHttpControllerCallableInfo
{
public:
    IHttpControllerCallableInfo(void *m_handler, const QString &m_className,
                              const QMap<QString, QString> &m_classInfos,
                              const QVector<QMetaMethod> &methods);
private:
    void parseMapppingInfos();
    void parseToActions();
    void findActionsProperties();
    void parseRootPaths();

private:
    std::vector<IHttpPathFragment> parseFragments(const QString& path);
    IHttpCallable getHttpMethodNode(const QString& name);

private:
    void checkPathArgumentMatch();
    void checkMappingOverloadFunctions();
    void checkMappingNameAndFunctionIsMatch();

private:
    void* m_handler{};
    QString m_className;
    QMap<QString, QString> m_classInfos;
    QVector<QMetaMethod> m_classMethods;

private:
    std::vector<IHttpPathFragment> rootFragments;
    QVector<IHttpCallableMappingInfo> m_mappingInfos;

public:
    QVector<IHttpControllerAction> m_actionNodes;
};

$PackageWebCoreEnd
