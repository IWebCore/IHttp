
#pragma once

#include "core/util/IPreProcessorUtil.h"
#include "core/util/IStringUtil.h"

#define $AsResponse(klassName)                                                                                                      \
public:                                                                                                                             \
    klassName(const klassName &rhs) : IHttpResponseInterface(rhs){}                                                                     \
    klassName(klassName&& rhs) : IHttpResponseInterface(std::move(rhs)){}                                                               \
    klassName& operator=(klassName &rhs){ IHttpResponseInterface::operator =(rhs);   return *this; }                                    \
    klassName& operator=(klassName &&rhs){  IHttpResponseInterface::operator =(std::move(rhs));  return *this; }                        \
private:                                                                                                                            \
    virtual IHttpResponseWare* prefixCreate(const std::string &data) final {                                                        \
        return new klassName(IStringUtil::mid(data, prefixMatcher().length()));                                                     \
    }
