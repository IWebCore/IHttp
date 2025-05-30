#pragma once

#include "core/util/IHeaderUtil.h"

$PackageWebCoreBegin

struct IHttpCachedSessionData
{
public:
    explicit IHttpCachedSessionData(const QString& key);

public:
    bool isExpired() const;
    bool isClearable(std::int64_t current) const;

    void remove(const QString& key);
    QVariant value(const QString& name, const QVariant& val = {}) const;
    void setValue(const QString& name, const QVariant& value);
    void updateLastVisitTime();

public:
    QString m_key;
    int64_t m_lastVisitTime;
    QVariantMap m_data;
};

$PackageWebCoreEnd

