#pragma once

#include "core/util/IPreProcessorUtil.h"
#include "core/util/IStringUtil.h"

// TODO: this problem will be solved in future?

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
        return new klassName(IStringUtil::mid(data, prefixMatcher().length()));                                                     \
    }

