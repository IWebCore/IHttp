#pragma once

#include "core/util/IPreProcessorUtil.h"
#include "core/util/IStringUtil.h"

#define $AsResponseNoPrefix(klassName)                                                                                              \
    public:                                                                                                                         \
        klassName(const klassName &rhs) : IHttpResponseInterface(rhs){}                                                             \
        klassName(klassName&& rhs) : IHttpResponseInterface(std::move(rhs)){}                                                       \
        klassName& operator=(const klassName &rhs){ IHttpResponseInterface::operator =(rhs);   return *this; }                      \
        klassName& operator=(klassName &&rhs){  IHttpResponseInterface::operator =(std::move(rhs));  return *this; }

#define $AsResponse(klassName)                                                                                                      \
    $AsResponseNoPrefix(klassName)                                                                                                  \
private:                                                                                                                            \
    virtual IHttpResponseWare* prefixCreate(const std::string &data) final {                                                        \
        static const bool overrided = typeid(& klassName ::prefixMatcher) != typeid(&IHttpResponseWare::prefixMatcher);             \
        if(overrided){                                                                                                              \
            return new klassName(IStringUtil::mid(data, prefixMatcher().length()));                                                 \
        }                                                                                                                           \
        return nullptr;                                                                                                             \
    }
