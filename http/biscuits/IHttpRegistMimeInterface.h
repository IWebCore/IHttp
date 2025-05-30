#pragma once

#include "core/util/IHeaderUtil.h"
#include "core/task/unit/ITaskInstantUnit.h"
#include "http/biscuits/IHttpMime.h"

$PackageWebCoreBegin

template<typename T, bool enabled = true>
class IHttpRegistMimeInterface : public ITaskInstantUnit<T, enabled>
{
public:
    IHttpRegistMimeInterface() = default;

public:
    virtual QMap<IString, IString> mimes() const = 0;

private:
    virtual void $task() final;
};

template<typename T, bool enabled>
void IHttpRegistMimeInterface<T, enabled>::$task()
{
    auto values = mimes();
    for(const auto& key : values.keys()){
        IHttpMimeUtil::registerSuffixMime(key, values[key]);
    }
}

$PackageWebCoreEnd
