#pragma once

#include "http/callable/IHttpReturnType.h"

$PackageWebCoreBegin

class IHttpReturnTypeDetail : public IHttpReturnType
{
public:
    IHttpReturnTypeDetail(QMetaType::Type type, const std::string& name);

private:
    void createResolveFuntion();

private:
    void createResponseFun();
    void createBeanFun();
    void createVoidFun();
    void createStatusFun();
    void createStdStringFun();
    void createQStringFun();
    void createQByteArrayFun();
    void createIStringFun();
    void createQStringListFun();
    void createIJsonFun();

private:
    void createQDateFun();
    void createQDateTimeFun();
    void createQTimeFun();

private:
    void createNumberFun();

private:
    static void setTextResponse(IHttpRequestImpl& impl, std::string&& text);   // 性能考虑
};

$PackageWebCoreEnd
