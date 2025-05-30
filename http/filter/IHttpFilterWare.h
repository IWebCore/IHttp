#pragma once

#include "core/util/IHeaderUtil.h"

$PackageWebCoreBegin

class IRequest;
class IHttpFilterWare
{
public:
    enum class Type{
        FirstLine,  // 解析首行
        Header,     // 解析完 header
        Body,       // 解析完 body
        PreHandle,  // pre method
        PostHandle, // post method
        completion,   // 完成 一个 请求发送过程
    };

public:
    IHttpFilterWare() = default;
    virtual ~IHttpFilterWare() = default;

public:
    virtual void filter(IRequest&) = 0;
    virtual Type type() const = 0;
};

$PackageWebCoreEnd
